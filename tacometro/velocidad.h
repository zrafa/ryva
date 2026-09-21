#ifndef VELOCIDAD_H
#define VELOCIDAD_H

#include <stdint.h>

/*
 * COMANDOS POSIBLES
 * 0x1A : reporta distancia (alta frecuencia > 100hz )
 * 0x1B : reporta velocidad (baja frecuencia: aprox entre 4hz a 10hz) 
 * 0x0N : establece distancia recorrida por flanco de taco
 *        Este ultimo comando estable cuanta distancia se recorrió por
 *        cada flanco de taco de la rueda. 
 *        Para calibrar: recorrer con el tractor X mts y contar Y flancos.
 *                       X * 100 / Y = Z cm recorrido por flanco.
 */

void velocidad_set_cm_x_taco(uint16_t n);
uint16_t velocidad(void);

#endif
