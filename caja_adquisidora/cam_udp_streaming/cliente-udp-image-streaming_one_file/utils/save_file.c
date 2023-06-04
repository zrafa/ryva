
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



int grabando = 0;
int fdo;
struct sigaction old_action;

void sigint_handler(int sig_no)
{
	printf("finalizamos..\n");
	close(fdo);
	sigaction(SIGINT, &old_action, NULL);
	kill(0, SIGINT);
}





void save_to_file(unsigned char * frame, int len)
{
	char nombre[64];

	if (! grabando) {

		grabando=1;

		//sprintf(nombre, "%s/%lld.jpg", dir_name, microsegundos());
		sprintf(nombre, "%s/%lld.jpg", dir_name, average_us);
		fdo = open(nombre, O_WRONLY | O_CREAT, 0644);
		if (fdo == -1) {
			fprintf(stderr, "open failed: %m\n");
			exit(1);
		}

		        /* para control C o SIGTERM (kill) */
		        struct sigaction action;
		        memset(&action, 0, sizeof(action));
		        action.sa_handler = &sigint_handler;
		        sigaction(SIGINT, &action, &old_action);
		        sigaction(SIGTERM, &action, NULL);
	}


	write(fdo, frame, len);
//	close(fdo);
}
