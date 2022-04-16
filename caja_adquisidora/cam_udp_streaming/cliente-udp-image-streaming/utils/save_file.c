
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

extern char *dir_name;
extern long long int average_us;

long long int microsegundos()
{
        struct timeval timer_usec;
        long long int timestamp_usec;
        if (!gettimeofday(&timer_usec, NULL)) {
                timestamp_usec = ((long long int) timer_usec.tv_sec) * 
                        1000000ll + (long long int) timer_usec.tv_usec;
        } else
                 timestamp_usec = -1;

        return timestamp_usec;
}


void save_to_file(unsigned char * frame, int len)
{
	char nombre[64];
	int fdo;

	//sprintf(nombre, "%s/%lld.jpg", dir_name, microsegundos());
	sprintf(nombre, "%s/%lld.jpg", dir_name, average_us);
	fdo = open(nombre, O_WRONLY | O_CREAT, 0644);
	if (fdo == -1) {
		fprintf(stderr, "open failed: %m\n");
		exit(1);
	}


	write(fdo, frame, len);
	close(fdo);
}
