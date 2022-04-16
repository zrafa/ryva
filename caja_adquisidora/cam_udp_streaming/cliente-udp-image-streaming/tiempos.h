
/*
 * Utilitario con funciones para medir el tiempo de ejecucion de 
 * diferentes partes del codigo
 *
 * rafa@fi.uncoma.edu.ar
 *
 */

#include <stdio.h>
#include <sys/time.h>

#include <stdio.h>
#include <sys/time.h>

#define ELAPSED frameEndTime.tv_sec - frameStartTime.tv_sec + \
             ((frameEndTime.tv_usec - frameStartTime.tv_usec)/1.0E6);


#ifndef TIEMPOS_H
#define TIEMPOS_H

void frame_start(void);
void frame_end();
void cronometro_start();
void cronometro_stop();
void msleep(long m);
long long int average_time();


#endif /* TIEMPOS_H */
