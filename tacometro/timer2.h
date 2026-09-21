/**********************************************************************
 *
 * Filename: timer2.h
 * 
 * API para la aplicacion embebida 
 * META: ocultar el hardware a la aplicacion 
 *
 **********************************************************************/

#include <stdint.h>

#ifndef _TIMER2_H
#define _TIMER2_H

void timer2_init(void);
uint32_t timer2_get_ticks();
void timer2_set_ticks(uint32_t t);

#endif /* _TIMER2_H */
