/*
 * main.c: little example program for testing garmin lidar lite v4
 * 
 * Rafael Ignacio Zurita (c) 2022 <rafa@fi.uncoma.edu.ar>
 */


#include <stddef.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "serial.h"
#include "twi.h"
#include "lidar-lite.h"

#define DELAY_180 450000

void esperar(long curr, long new_pos)
{
	volatile long delay;
	volatile int i;

	if (curr < new_pos)
		delay = ((new_pos - curr) * DELAY_180) / 180;
	else
		delay = ((curr - new_pos) * DELAY_180) / 180;

	for(i=0; i<(int)delay; i++);
}

void get_and_show_distance(int j)
{
	char msg[80];
	uint16_t distance;
	unsigned int ticks;
	int i;

	timer_set_ticks(0);
	distance = lidar_v4_get_distance();
	ticks = timer_get_ticks();
	//sprintf(msg, "distance (cm): %i \n", distance);
	sprintf(msg, "%.3d:%.5d:%.6d", j, distance, ticks);
	serial_put_str(msg);
}

void main()
{
	/* pin 9 arduino pwm signal output */
        DDRB = 0x02;

	volatile unsigned long i;
	int j;
	int current;

	serial_init(115200);
	twi_init();		/* init i2c */
	lidar_v4_init();	/* init lidar with some specific values (check code) */
	/* servo init */
        timer1_init();
	mover(0);
	current = 0;
	for(i = 0; i < 450000;i++);

	int movimiento = 0;
	char c = '0';
	while(1) {
		if (serial_recibido() ) {
			c = serial_get_char();
			if (c == '0')
				movimiento = 0;
			else if (c == '1')
				movimiento = 1;
		}

		if (movimiento == 0) {
			get_and_show_distance(0);
			continue;
		}

		for (j=0; j<=180; j=j+10) {
			mover(j);
			for(i = 0; i < 15000;i++);
			get_and_show_distance(j);
		}
		for (j=175; j>=0; j=j-10) {
			mover(j);
			for(i = 0; i < 15000;i++);
			get_and_show_distance(j);
		}

	}
}


