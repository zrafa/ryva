
/*
 * Aplicacion embebida que realiza un testing hello world de los drivers
 */

#include <util/delay.h>
#include "gpio.h"
#include "serial.h"
#include "ultrasound.h"
#include "tacometro.h"

void main()
{
	uint16_t val;

	int estado_anterior = -1;

	int n = 0;

	serial_init();
		// serial_put_str("hola mundo \n\r");

	while (1) {
		/* obtener una distancia del ultrasound */
		// serial_put_str("    Ultrasound : \n\r");
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
		} else if ((estado ==0) && (estado != estado_anterior)) {
			estado_anterior = estado;
			serial_put_str("media up: ");
			serial_put_int(media_up, 4);
			serial_put_str("    media down: ");
			serial_put_int(media_down, 4);
			serial_put_str("    muestra: ");
			serial_put_int(val, 4);
			serial_put_str("      NO  TACO \n\r");
		}

		//serial_put_int(val, 4);
		//serial_put_str("\n\r");

/*
		serial_put_int(val, 4);
		serial_put_str("\n\r");
		n++;
		if (n==100) {
		   n=0;
			serial_put_str("Media up: ");
			serial_put_int(media_up, 4);
			serial_put_str("    Media down: ");
			serial_put_int(media_down, 4);
			serial_put_str("    muestra: ");
			serial_put_int(val, 4);
			_delay_ms(3000);
		}
*/
		_delay_ms(10);
	}
}
