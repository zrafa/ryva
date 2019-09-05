#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <getopt.h>

#include "config.h"
#include "tiempos.h"

long unsigned int YUYVtoJPEG(unsigned char* img, int width, int height, int jpegQuality, unsigned char* buf);
int init_socket(char *hostname, int portno);
void send_frame(unsigned char * frame, int len);

static int debug = 0;

static char *dev_name                   = "/dev/video0";
static int fd                           = -1;
static void *buffer_start               = NULL;
static size_t length                    = 0;
static unsigned char *jpegbuffer_start  = NULL;
static int portno                       = 8000;
// static char *hostname                   = "127.0.0.1";
static char *hostname                   = "10.0.40.42";
//static char *hostname                   = "10.0.40.99";
static struct v4l2_format fmt;


#define CLEAR(x) memset(&(x), 0, sizeof(x))


struct buffer {
        void   *start;
        size_t  length;
};

// static enum io_method   io = IO_METHOD_MMAP;
struct buffer          *buffers;
static unsigned int     n_buffers;
static int              out_buf;
static int              force_format;
static int              frame_count = 70;


static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}





// necessary to silent clang warning
static void errno_exit(const char *s) __attribute__((noreturn));

static void errno_exit(const char *s) 
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}
int size; 
static void send_YUV(const void *p, int size)
{
//	long unsigned int size;
int size2;

cronometro_start();
  // size = YUYVtoJPEG(buffer_start, fmt.fmt.pix.width, fmt.fmt.pix.height, ENCODE_QUALITY, jpegbuffer_start);
  size2 = YUYVtoJPEG(p, fmt.fmt.pix.width, fmt.fmt.pix.height, ENCODE_QUALITY, jpegbuffer_start);
	if (debug) {
		printf("send_YUV: size %ld\n", size);
	}
printf("Convertir: ");
cronometro_stop();


cronometro_start();
  send_frame(jpegbuffer_start, size2);
printf("Enviar: ");
cronometro_stop();

}

static int jpegsize()
{
    int size, i;

    unsigned char * buf = buffer_start;

    for (i = length-1; i >= 0; i--) {
        if ((buf[i] << 8 | buf[i+1]) == 0xFFD9)
            break;
    }

    size = i+2;

    return size;
}

static void send_MJPEG()
{
//    int size;

	if (debug) {
		printf("send_MJPEG\n");
	}

 //   size = jpegsize();

    send_frame(buffer_start, size);

}

static int read_frame()
{
	struct v4l2_buffer buf;

	unsigned int i;


/*
	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	cronometro_start();
	if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
		printf("ioctl dqbuf is wrong !!!\n");
		switch (errno) {
			case EAGAIN:
				return 0;
			case EIO:
*/
				/* Could ignore EIO, see spec. */
				/* fall through */
/*
			default:
				errno_exit("VIDIOC_DQBUF");
		}
	}
	printf("Leer frame: ");
	cronometro_stop();

*/

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

//                process_image(buffers[buf.index].start, buf.bytesused);
	buffer_start = buffers[buf.index].start;
	size = buf.bytesused;
	printf ("ultimo byte:%X-", buffer_start+(size-1));



	cronometro_start();
		printf("222222222222 mPEGGGGG ");
	if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {

		printf("YUYVVVVVV ");
    		// send_YUV();
    		send_YUV(buffers[buf.index].start, buf.bytesused);

	} else if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) {
		printf("mPEGGGGG ");

    		send_MJPEG();
  	}
	printf("Enviar frame: ");
	cronometro_stop();

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                        errno_exit("VIDIOC_QBUF");


/*
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
		errno_exit("VIDIOC_QBUF");
*/

	return 0;
}

int fps = 0;

static void mainloop(void)
{
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
      /* exit(EXIT_FAILURE); */
      continue;
    }
	printf("Espera por datos disponibles en /dev/video0: ");
	cronometro_stop();

    if (read_frame())
      break;
    // EAGAIN - continue select loop.

    fps++;
    printf(" FPS:%i ",fps);
    // frame_end();
    // frame_start();
  }
}

static void stop_capturing(void)
{
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1)
    errno_exit("VIDIOC_STREAMOFF");
}

static void start_capturing(void)
{
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

//  struct v4l2_buffer buf;
//  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;


/*
  memset(&buf, 0, sizeof(buf));
  buf.type = type;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;

  if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
    errno_exit("VIDIOC_QBUF ... !!!");

  if (ioctl(fd, VIDIOC_STREAMON, &type) == -1)
    errno_exit("VIDIOC_STREAMON");
*/
}

static void uninit_device(void)
{
  unsigned int i;

                for (i = 0; i < n_buffers; ++i)
                        if (-1 == munmap(buffers[i].start, buffers[i].length))
                                errno_exit("munmap");
   free(buffers);

//  if (munmap(buffer_start, length) == -1)
//    errno_exit("munmap");

}

static void init_mmap(void)
{

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





/*
  struct v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  req.count  = 1;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1)
    errno_exit("VIDIOC_REQBUFS");

  struct v4l2_buffer buf;
  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;

  if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
    errno_exit("VIDIOC_QUERYBUF");

  length = buf.length;
  buffer_start = mmap(NULL /* start anywhere */   //,
//                      length,
//                      PROT_READ | PROT_WRITE /* required */,
//                      MAP_SHARED /* recommended */,
//                      fd, buf.m.offset);
//
//  if (buffer_start == MAP_FAILED)
//    errno_exit("mmap");


}

static void get_pixelformat()
{
  struct v4l2_fmtdesc desc;
  memset(&desc, 0, sizeof(desc));
  desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  // iterate over all formats, and prefer MJPEG when available
  while (ioctl(fd, VIDIOC_ENUM_FMT, &desc) == 0) {
    desc.index++;

    if (desc.pixelformat == V4L2_PIX_FMT_MJPEG) {
      fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
      printf("Using: MJPEG\n");
      return;
    }
  }

  // printf("Using: YUYV\n");
  printf("Using: MJPEG\n");
}

static void init_device(void)
{

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
	force_format = 1;
        if (force_format) {
                fmt.fmt.pix.width       = 640;
                fmt.fmt.pix.height      = 480;
                // fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
                fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
                // RAFA fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
                fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

                if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
                        errno_exit("VIDIOC_S_FMT");

                /* Note VIDIOC_S_FMT may change width and height. */

        } else {
                /* Preserve original settings as set by v4l2-ctl for example */
                if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
                        errno_exit("VIDIOC_G_FMT");
        }

        /* Buggy driver paranoia. */
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                fmt.fmt.pix.sizeimage = min;



                init_mmap();






/*
  struct v4l2_capability cap;

  memset(&cap, 0, sizeof(cap));
  if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1)
    errno_exit("VIDIOC_QUERYCAP");

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s is no video capture device\n", dev_name);
    exit(EXIT_FAILURE);
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    fprintf(stderr, "%s does not support streaming i/o\n", dev_name);
    exit(EXIT_FAILURE);
  }

  // Default to YUYV o MJPEG? :)
  memset(&fmt, 0, sizeof(fmt));
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  // RAFA fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  get_pixelformat();

  // it'll adjust to the bigger screen available in the driver
  fmt.fmt.pix.width  = FRAME_WIDTH;
  fmt.fmt.pix.height = FRAME_HEIGHT;

  if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
  	errno_exit("VIDIOC_S_FMT");

  init_mmap ();

*/
}

static void close_device(void)
{
  if (close (fd) == -1)
    errno_exit("close");
}

static void open_device(void)
{
  if ((fd = open(dev_name, O_RDWR)) == -1) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno,
            strerror (errno));
    exit(EXIT_FAILURE);
  }
}


/*
  print usage information
*/
static void usage(FILE* fp, int argc, char** argv)
{
  fprintf (fp,
    "Usage: %s [options]\n\n"
    "Options:\n"
    "-d | --device name       Video device name [/dev/video0]\n"
    "-h | --help              Print this message\n"
    "-s | --server address    Server [ip address/hostname]\n"
    "-p | --port port number  Port [8888]\n"
   "",
    argv[0]);
}

static const char short_options [] = "d:h:s:p:";

static const struct option
long_options [] = {
        { "device",     required_argument,      NULL,           'd' },
        { "help",       no_argument,            NULL,           'h' },
        { "server",     required_argument,      NULL,           's' },
        { "port",       required_argument,      NULL,           'p' },
       { 0, 0, 0, 0 }
};



int main(int argc, char **argv)
{

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
