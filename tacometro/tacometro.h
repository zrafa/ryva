

#ifndef TACOMETRO_H
#define TACOMETRO_H

extern uint8_t estado;
extern double media_up;
extern double media_down;

/*
 * Llamar una vez por cada muestra.
 *
 * Aprende las dos mesetas mediante dos medias adaptativas.
 * No supone ningún valor concreto del sensor.
 */
void calibrar(uint16_t x);
uint8_t detectar_taco(uint16_t x, uint16_t muestras_necesarias);

#endif
