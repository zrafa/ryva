/**********************************************************************
 *
 * Filename:    serial.h
 * 
 * API para la aplicacion embebida 
 * META : ocultar el hardware a la aplicacion 
 *
 **********************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

void serial_init(uint32_t USART_BAUDRATE);
void serial_put_char(char);
char serial_get_char(void);
void serial_put_str(char *);
int serial_recibido(void);

#endif /* _SERIAL_H */
