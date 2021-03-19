/*
 * Utilitario con funciones para medir el tiempo de ejecucion de 
 * diferentes partes del codigo
 *
 * rafa@fi.uncoma.edu.ar
 *
 */

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#define ELAPSED frame_end_time.tv_sec - frame_start_time.tv_sec + \
             ((frame_end_time.tv_usec - frame_start_time.tv_usec)/1.0E6);


struct timeval frame_start_time, frame_end_time;

void frame_start(void) {
  gettimeofday(&frame_start_time, NULL);
}

void frame_end() {

  float elapsed_time; 
  char str[30]; 

  gettimeofday(&frame_end_time, NULL);

  elapsed_time= ELAPSED;
             
  sprintf(str, "Frames per second: %2.0f", 1.0/elapsed_time);
  printf("%s \r", str);
  fflush(0);

}

struct timeval t1, t2;

long long int average_time()
{

	long long int t1_usec, t2_usec;
	t1_usec = ((long long int) t1.tv_sec) * 
			1000000ll + (long long int) t1.tv_usec;
	t2_usec = ((long long int) t2.tv_sec) * 
			1000000ll + (long long int) t2.tv_usec;

	return (long long int) (t1_usec+t2_usec/2);
}


void cronometro_start() {
	gettimeofday(&t1, NULL);
}

void cronometro_stop() {

	double elapsed_time;

	gettimeofday(&t2, NULL);
	elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
	printf(" %f ms ", elapsed_time);
}


int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
