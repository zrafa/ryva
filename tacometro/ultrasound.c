/*
 * ultrasound: driver de ultrasonido
 */

/* 
 * get_distance: devuelve la distancia (en cm) del objeto enfrente.
 */
#include "gpio.h"
#include <util/delay.h>

#define TIME_OUT_US 36000
#define TRIGGER_PULSE_WIDE_US 10
// #define TIME_DISTANCE_RELATION 58
#define TIME_DISTANCE_RELATION 30
#define TIME_BETWEEN_SCANS_US 1


int ultrasound_get(int trig, int echo)
{
    double cm;
    long int tiempo; 

    gpio_input(echo);
    gpio_output(trig);

    //Pulso de 10us para la señal de trigger
    gpio_pin(trig, ON);
    _delay_us(TRIGGER_PULSE_WIDE_US);
    gpio_pin(trig, OFF); 

    //Echo lee la señal
    tiempo = 0;
    while (gpio_pin(echo, GET) == 0);
    while ((gpio_pin(echo, GET) != 0) && (tiempo < TIME_OUT_US)) {//Espera a que el pin cambie de estado
        _delay_us(TIME_BETWEEN_SCANS_US);
        tiempo += TIME_BETWEEN_SCANS_US;
    }
        
    //Calculo de la distancia
    if (tiempo >= TIME_OUT_US) {
        cm = -1;
    } else {
        cm = (double) tiempo / (double) TIME_DISTANCE_RELATION;
    }   

    return (cm * 100);		// devolvemos mm en vez de cm
}

