/* Driver del timer2 del atmega328p
 *
 * Author: Lucas Magni (estudiante pse2026)
 *
 * Genera 1000 interrupciones por segundo
 * Simula 100 interrupciones por segundo, actualizando el valor de ticks cada 10 ticks reales
 *
 * Modo CTC, incrementa el contador hasta el valor definido en el registro comparador A
 *
 * Reloj del sistema: 16000000 ticks/s
 * Preescalar: 64
 * 
 * 16000000 ticks/s / 64 = 250000 ticks/s
 * 250000 ticks/s / 1000 tick/s = 250
 * 
 * Valor de registro comparador A: 250 (por la cuenta de arriba)
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include "timer2.h"

volatile uint32_t ticks = 0;

/* Estructura de datos del driver timer2 */
typedef struct {
    uint8_t tccr2a;     /* Timer Control Register A */
    uint8_t tccr2b;     /* Timer Control Register B */
    uint8_t tcnt2;      /* Timer Register */
    uint8_t ocr2a;      /* Output Compare Register A */
    uint8_t ocr2b;      /* Output Compare Register B */
} volatile timer2_t;

volatile timer2_t *timer2 = (timer2_t *) 0xB0;

/* Interrupt Mask Register */
volatile uint8_t *timer2_timsk2 = (uint8_t *) 0x70;

/* Inicializar driver timer2 */
void timer2_init() {
    /* modo CTC */
    timer2->tccr2a |= 0x02;

    /* preesacalar = 64 */
    timer2->tccr2b |= 0x04;

    /* valor maximo del contador */
    timer2->ocr2a = 250;

    /* genera interrupcion si contador es igual a registro OCR2A */
    (*timer2_timsk2) |= 0x02;

    /* habilitar las interrupciones a nivel global */
    sei();
}

/* Rutina de atencion de interrupciones */
ISR(TIMER2_COMPA_vect) {
	ticks++;
}

/* Devolver la cantidad de ticks actuales */
uint32_t timer2_get_ticks() {
	uint32_t t;

	cli();
	t = ticks;
	sei();
	
    return t;
}

void timer2_set_ticks(uint32_t t)
{
	cli();
	ticks = t;
	sei();
}

