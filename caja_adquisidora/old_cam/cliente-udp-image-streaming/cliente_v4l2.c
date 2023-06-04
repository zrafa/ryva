/* 
 * cliente_v4l2.c : Programa que captura imagenes y envia las mismas via UDP.
 *                  Puede utilizarse para streaming o procesamiento 
 *                  en tiempo real. 
 *                  Esta pensado para sistemas embebidos,
 *                  por lo que no utiliza bibliotecas externas. 
 *                  
 * Changelog:
 *      2017: Version inicial por Rodolfo del Castillo
 *            utilizando codigo fuente de ejemplo de Video For Linux 2
 *      2019: Rafael Ignacio Zurita (rafa@fi.uncoma.edu.ar), mejoras
 *            en la performance general, utilizando codigo desde:
 *            https://www.linuxtv.org/downloads/v4l-dvb-apis-new/media.pdf,
 *            mplayer, y modificaciones locales.
 */       

#include <assert.h>
#include <errno.h>
#include <fcntl.h>              /* low-level i/o */
#include <getopt.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "tiempos.h"
#include "utils/save_file.h"

long unsigned int YUYVtoJPEG(unsigned char * img, int width, int height, int jpegQuality, unsigned char* buf);
int init_socket(char *hostname, int portno);
void send_frame(unsigned char * frame, int len);


static char *dev_name                   = "/dev/video0";
static int fd                           = -1;
static void *buffer_start               = NULL;
static unsigned char *jpegbuffer_start  = NULL;
static int portno                       = 8000;
// static char *hostname                   = "127.0.0.1";
static char *hostname                   = "10.0.40.42";
char *dir_name                   = "/mnt/disk";
//static char *hostname                   = "10.0.40.99";
static struct v4l2_format fmt;

/* Variable global que almacena la cantidad de bytes obtenidos desde la camara */
int size; 

/* Variable global que almacena la cantidad total de frames obtenidos */
int total_frames = 0;

int jpeg = 0;
int save = 0;
int resolucion = 0;

#define CLEAR(x) memset(&(x), 0, sizeof(x))

long long int average_us;

struct buffer {
        void   *start;
        size_t  length;
};

struct buffer          *buffers;
static unsigned int     n_buffers;


static int xioctl(int fh, int request, void *arg) {
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}





// necessary to silent clang warning
static void errno_exit(const char *s) __attribute__((noreturn));


static void errno_exit(const char *s) {

  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}


static void send_YUV(unsigned char *p, int size) {

	int size2;

	cronometro_start();
	size2 = YUYVtoJPEG(p, fmt.fmt.pix.width, fmt.fmt.pix.height, ENCODE_QUALITY, jpegbuffer_start);
	printf("Convertir: ");
	cronometro_stop();

	cronometro_start();
	if (save)
		save_to_file(jpegbuffer_start, size2);
	send_frame(jpegbuffer_start, size2);
	printf("Enviar: ");
	cronometro_stop();

}


static void send_MJPEG() {

	cronometro_start();
	if (save)
		save_to_file(buffer_start, size);
	send_frame(buffer_start, size);
	printf("Enviar: ");
	cronometro_stop();
}


static int read_frame() {

	struct v4l2_buffer buf;

	cronometro_start();
	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
		case EAGAIN:
			return 0;

		case EIO:
			/* Could ignore EIO, see spec. */
			/* fall through */

		default:
			errno_exit("VIDIOC_DQBUF");
		}
	}

	assert(buf.index < n_buffers);
	buffer_start = buffers[buf.index].start;
	size = buf.bytesused;

	cronometro_stop();
	average_us = average_time();

	if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {

    		send_YUV(buffers[buf.index].start, buf.bytesused);

	} else if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) {

    		send_MJPEG();
  	}

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
		errno_exit("VIDIOC_QBUF");

	return 0;
}



static void stop_capturing(void) {

  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1)
    errno_exit("VIDIOC_STREAMOFF");
}


static void start_capturing(void) {

	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");
}


static void uninit_device(void) {

	unsigned int i;

	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap(buffers[i].start, buffers[i].length))
			errno_exit("munmap");
	free(buffers);
}


static void init_mmap(void) {

        struct v4l2_requestbuffers req;

        CLEAR(req);

        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s does not support "
                                 "memory mappingn", dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2) {
                fprintf(stderr, "Insufficient buffer memory on %s\\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }

        buffers = calloc(req.count, sizeof(*buffers));

        if (!buffers) {
                fprintf(stderr, "Out of memory\\n");
                exit(EXIT_FAILURE);
        }


        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                        errno_exit("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;
                buffers[n_buffers].start =
                        mmap(NULL /* start anywhere */,
                              buf.length,
                              PROT_READ | PROT_WRITE /* required */,
                              MAP_SHARED /* recommended */,
                              fd, buf.m.offset);

                if (MAP_FAILED == buffers[n_buffers].start)
                        errno_exit("mmap");
        }
}


static void init_device(void) {

        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        // RAFA struct v4l2_format fmt;
        unsigned int min;

        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
                if (EINVAL == errno) {
                        fprintf(stderr, "%s is no V4L2 device\\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                } else {
                        errno_exit("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
                fprintf(stderr, "%s is no video capture device\\n",
                         dev_name);
                exit(EXIT_FAILURE);
        }

                if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
                        fprintf(stderr, "%s does not support streaming i/o\\n",
                                 dev_name);
                        exit(EXIT_FAILURE);
                }

        /* Select video input, video standard and tune here. */


        CLEAR(cropcap);

        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                crop.c = cropcap.defrect; /* reset to default */

                if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {

                        switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                                break;
                        default:
                                /* Errors ignored. */
                                break;
                        }
                }
        } else {
                /* Errors ignored. */
        }


        CLEAR(fmt);

        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (jpeg)
                fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	else
                fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	
	fmt.fmt.pix.width       = 160;
	fmt.fmt.pix.height      = 120;

	switch (resolucion) {
	  case 3:
		fmt.fmt.pix.width       = 320;
		fmt.fmt.pix.height      = 240;
		break;
	  case 6:
		fmt.fmt.pix.width       = 640;
		fmt.fmt.pix.height      = 480;
		break;
	  case 7:
		fmt.fmt.pix.width       = 1280;
		fmt.fmt.pix.height      = 720;
		break;
	  case 9:
		fmt.fmt.pix.width       = 1920;
		fmt.fmt.pix.height      = 1080;
		break;
	};
	  

	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT");

        /* Buggy driver paranoia. */
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;

	init_mmap();
}


static void close_device(void) {

  if (close (fd) == -1)
    errno_exit("close");
}


static void open_device(void) {

  if ((fd = open(dev_name, O_RDWR)) == -1) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno,
            strerror (errno));
    exit(EXIT_FAILURE);
  }
}

static void mainloop(void) {

  int nro_timeouts = 0;

  for (;;) {

    fd_set fds;
    struct timeval tv;
    int r;

    cronometro_start();

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    tv.tv_sec = 6;
    tv.tv_usec = 0;

    r = select(fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r)
    {
      if (EINTR == errno)
        continue;

      errno_exit("select");
    }

    if (0 == r)
    {
      fprintf(stderr, "select timeout\n");
      /* RAFA: en mplayer no falla, vuelve a reintentar con continue */
      /* RAFA: igualmente reinicio la captura, como prueba */
      /* exit(EXIT_FAILURE); */

	nro_timeouts++;
	if (nro_timeouts == 3) {
		nro_timeouts = 0;

		stop_capturing();
		start_capturing();

	}
	continue;
    }

    printf("\rEspera en /dev/video0: ");
    cronometro_stop();

    if (read_frame())
        break;
    	// EAGAIN - continue select loop.

    total_frames++;
    printf(" Total frames:%i ",total_frames);
  }
}


/*
  print usage information
*/
static void usage(FILE* fp, int argc, char** argv) {

  fprintf (fp,
    "Usage: %s [options]\n\n"
    "Options:\n"
    "-d | --device name       Video device name [/dev/video0]\n"
    "-h | --help              Print this message\n"
    "-s | --server address    Server [ip address/hostname]\n"
    "-p | --port port number  Port [8888]\n"
    "-w | --save dir 	      Save photos to disk (dir name)\n"
    "-j | --jpeg 	      formato MJPEG (predeterminado YUYV)\n"
    "                     (solo para la captura, el envio es siempre en formato JPEG).\n"
    "--320 	      res: 160x120 (predeterminado: 160x120)\n"
    "--640 	      res: 160x120 (predeterminado: 160x120\n"
    "--720 	      res: 1280x720 (predeterminado: 160x120)\n"
    "--1080 	      res: 1920x1080 (predeterminado: 160x120)\n"
   "",
    argv[0]);
}

#define M320  1000
#define M640  1001
#define M720  1002
#define M1080  1003

static const char short_options [] = "d:h:s:p:j:NULL:NULL:NULL:NULL:";

static const struct option
long_options [] = {
        { "device",     required_argument,      NULL,           'd' },
        { "help",       no_argument,            NULL,           'h' },
        { "server",     required_argument,      NULL,           's' },
        { "port",       required_argument,      NULL,           'p' },
        { "save",       required_argument,      NULL,           'w' },
        { "jpeg",       no_argument, 	        NULL,           'j' },
        { "320",       no_argument, 	        NULL,           M320 },
        { "640",       no_argument, 	        NULL,           M640 },
        { "720",       no_argument, 	        NULL,           M720 },
        { "1080",       no_argument, 	        NULL,           M1080 },
	{NULL, 0, NULL, 0}
};


int main(int argc, char **argv) {

  for (;;) {
    int index, c = 0;
                
    c = getopt_long(argc, argv, short_options, long_options, &index);

    if (-1 == c)
      break;

    switch (c) {
      case 0: /* getopt_long() flag */
        break;

      case 'd':
        dev_name = optarg;
        break;

      case 'h':
        // print help
        usage (stdout, argc, argv);
        exit(EXIT_SUCCESS);

      case 's':
        // server hostname or ip address
        hostname = optarg;
        break;

      case 'p':
        // port number
        portno = atoi(optarg);
        break;

      case 'w':
        dir_name = optarg;
	save = 1;
        break;

      case 'j':
        // jpeg (default yuyv)
        jpeg = 1;
        break;

      case M320:
        resolucion = 3;
        break;

      case M640:
        resolucion = 6;
        break;

      case M720:
        resolucion = 7;
        break;

      case M1080:
        resolucion = 9;
        break;


      default:
        usage(stderr, argc, argv);
        exit(EXIT_FAILURE);
    }
  }

  printf("video: %s\n", dev_name);
  printf("serverip: %s\n", hostname);
  printf("port: %d\n", portno);

  init_socket(hostname, portno);

  open_device();
  init_device();

  jpegbuffer_start = (unsigned char *)malloc(fmt.fmt.pix.width*fmt.fmt.pix.height*3);	

  start_capturing();
  mainloop();
  stop_capturing();
  uninit_device();
  close_device();

  free(jpegbuffer_start);
  return 0;
}
