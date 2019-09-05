
/*
 * Medir la cantidad de frames por segundo
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


#endif /* TIEMPOS_H */
