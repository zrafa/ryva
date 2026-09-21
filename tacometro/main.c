
/*
 * Aplicacion de tacometro
 *
 * COMANDOS POSIBLES por SERIAL
 * 0x1A : reporta distancia (alta frecuencia > 100hz )
 * 0x1B : reporta velocidad (baja frecuencia: aprox entre 4hz a 10hz) 
 * 0x0N : establece distancia recorrida por flanco de taco
 *        Este ultimo comando estable cuanta distancia se recorrió por
 *        cada flanco de taco de la rueda. 
 *        Para calibrar: recorrer con el tractor X mts y contar Y flancos.
 *                       X * 100 / Y = Z cm recorrido por flanco.
 */

#include <util/delay.h>
#include "gpio.h"
#include "serial.h"
#include "ultrasound.h"
#include "tacometro.h"

#define REPORTAR_DISTANCIA_CMD 0x1A
#define REPORTAR_VELOCIDAD_CMD 0x1B
#define SET_DISTANCIA_X_TACO_CMD 7		


void main()
{
	uint16_t val;
	char comando = -1;

	int estado_anterior = -1;

	int n = 0;

	serial_init();
	timer2_init();
	sei();

	while (1) {
		if (serial_rx_data()) { // obtuvimos un comando
			serial_cli_rx_data();
			comando = serial_get_char();
			if ((comando & (1 << SET_DISTANCIA_X_TACO_CMD)) == 0)  {
				comando = comando & 0x7F;
				velocidad_set_cm_x_taco(comando);
			} else if ((comando == REPORTAR_DISTANCIA_CMD) || (comando == REPORTAR_VELOCIDAD_CMD)) {
				tacometro_set_mode(comando);
			}
		}

		/* obtener una distancia del ultrasound */
 		val = (uint16_t) ultrasound_get(6, 5);	// D3: echo, D2: trigger
		
		calibrar((uint16_t) val);

		estado = detectar_taco((uint16_t) val, 4);

		if ((estado == 1) && (estado != estado_anterior)) {
			estado_anterior = estado;
			serial_put_str("media up: ");
			serial_put_int(media_up, 4);
			serial_put_str("    media down: ");
			serial_put_int(media_down, 4);
			serial_put_str("    muestra: ");
			serial_put_int(val, 4);
			serial_put_str(" TACO\n\r");
		} else if ((estado == 0) && (estado != estado_anterior)) {
			estado_anterior = estado;
			serial_put_str("media up: ");
			serial_put_int(media_up, 4);
			serial_put_str("    media down: ");
			serial_put_int(media_down, 4);
			serial_put_str("    muestra: ");
			serial_put_int(val, 4);
			serial_put_str("      NO  TACO \n\r");
		}

		_delay_ms(10);
	}
}
