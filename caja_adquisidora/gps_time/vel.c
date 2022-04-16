
/* velocidad a partir de latitud y longitud de GPS
 *
 * Rafael Zurita
 */


#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>




/* formato de los datos :
 *        Latitud,Longitud. Ej: 3849.84448,06804.07505
 */

#define PI 3.14159265358979323846
	
char *line = NULL;

struct sigaction old_action;

void sigint_handler(int sig_no)
{
    printf("finalizamos..\n");
    free(line);
    sigaction(SIGINT, &old_action, NULL);
    kill(0, SIGINT);
}



double get_x(const char *s)
{
	double x;
	char temp[20];
	strncpy(temp, s, 2);
	temp[2] = '\0';
	x = atof(temp);
	
	strncpy(temp, s+2, 8);
	temp[8] = '\0';
	x = x + (atof(temp) / 60.0);
	x*=-1;
	
	x = x * PI / 180.0;
	return x;
}

double get_y(const char *s)
{
	double y;
	char temp[20];
	strncpy(temp, s+11, 3);
	temp[3] = '\0';
	y = atof(temp);
	
	strncpy(temp, s+14, 8);
	temp[8] = '\0';
	y = y + (atof(temp) / 60.0);
	y*=-1;
	
	y = y * PI / 180.0;
	return y;
}

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



void main() {

        line = malloc(80);
	size_t len = 0;
	ssize_t line_size = 0;


	double lat1, lon1, lat2, lon2;
	double dlon, dlat, a, c, cm, km;

	long long int m1, m2, m;

       /* para control C o SIGTERM (kill) */
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        action.sa_handler = &sigint_handler;
        sigaction(SIGINT, &action, &old_action);
        sigaction(SIGTERM, &action, NULL);



	line_size = getline(&line, &len, stdin);
	m1 = microsegundos();
	lat1 = get_x(line);
	lon1 = get_y(line);

	for (;;) {
		line_size = getline(&line, &len, stdin);
		m2 = microsegundos();
		lat2 = get_x(line);
		lon2 = get_y(line);
	
		/* haversine formula */
		dlon = lon2 - lon1; 
		dlat = lat2 - lat1; 
		a = pow (sin(dlat/2), 2) + cos(lat1) * cos(lat2) * pow (sin(dlon/2), 2);
		c = 2 * asin(sqrt(a));
		/* Radius of earth in kilometers is 6371 */
		km = 6371.0 * c;
		cm = km * 1000.0 * 1000.0;

		/* tiempo transcurrido en milisegundos */
		m = (m2 - m1) / 1000.0;

		if (cm != 0)
			printf("distancia (cm): %f,  velocidad (cm/seg): %f \n", cm, (double) (cm / (m / 1000.0)));

		lat1 = lat2;
		lon1 = lon2;
		m1 = m2;
	}
	

}



