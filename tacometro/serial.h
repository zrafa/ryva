/**********************************************************************
 *
 * serial.h: interfaz del driver serial
 *
 **********************************************************************/

#ifndef _SERIAL_H
#define _SERIAL_H

void serial_init(void);
void serial_put_char(char c);
char serial_get_char(void);
void serial_put_str(char *str);
char serial_get_str(char * buffer, int max_string_length);
void serial_put_int (int v, int cant_digitos);
void serial_put_long_int (long int v, int cant_digitos);
void serial_put_double (double v, char digitos_enteros, char digitos_fraccionarios);
char serial_recibido(void);

#endif /* _SERIAL_H */
