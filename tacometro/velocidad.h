#ifndef VELOCIDAD_H
#define VELOCIDAD_H

#include <stdint.h>

/*
 * COMANDOS POSIBLES
 * 0x1A : reporta distancia (alta frecuencia > 100hz )
 * 0x1B : reporta velocidad (baja frecuencia: aprox entre 4hz a 10hz) 
 * 0b0XXXXXXX : establece distancia recorrida por flanco de taco                                                                   
 *        Este ultimo comando estable cuanta distancia se recorrió por                                                             
 *        cada flanco de taco de la rueda. EN mm/s.                                                                                
 *        Para calibrar: recorrer con el tractor X mts y contar Y flancos.                                                         
 *                       X * 1000 / Y = Z mm recorrido por flanco.   
 */

void tacometro_set_mode(char m);
char tacometro_get_mode(void);
void velocidad_set_mm_x_taco(uint16_t n);
uint16_t velocidad(void);
void velocidad_print(void);

#endif
