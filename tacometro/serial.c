
/**********************************************************************
 *
 * serial.c - Driver del UART del atmega328p
 *
 * Configuracion: 9600bps, 8bits data, 1bit stop, sin bit de paridad
 *
 **********************************************************************/

#include <stdint.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE  (((F_CPU/(USART_BAUDRATE*16UL)))-1)

/* UCSR0A - USART Control and Status Register A bits */
#define UCSR0A_MPCM0 0
#define UCSR0A_U2X0 1
#define UCSR0A_UPE0 2
#define UCSR0A_DOR0 3
#define UCSR0A_FE0 4
#define UCSR0A_UDRE0 5
#define UCSR0A_TXC0 6
#define UCSR0A_RXC0 7

/* UCSR0B - USART Control and Status Register B bits */
#define UCSR0B_TXB80 0
#define UCSR0B_RXB80 1
#define UCSR0B_UCSZ02 2
#define UCSR0B_TXEN0 3
#define UCSR0B_RXEN0 4
#define UCSR0B_UDRIE0 5
#define UCSR0B_TXCIE0 6
#define UCSR0B_RXCIE0 7

/* UCSR0C - USART Control and Status Register C bits */
#define UCSR0C_UCPOL0 0
#define UCSR0C_UCSZ00 1
#define UCSR0C_UCSZ01 2
#define UCSR0C_USBS0 3
#define UCSR0C_UPM00 4
#define UCSR0C_UPM01 5
#define UCSR0C_UMSEL00 6
#define UCSR0C_UMSEL01 7

#define RX_E (1 << UCSR0B_RXEN0)		   /* Habilitar la recepción */
#define TX_E (1 << UCSR0B_TXEN0)		   /* Habilitar la transmisión */
#define PARITY_MODE (0 << UCSR0C_UPM00)	   /* No usar bit de paridad */
#define STOP_BITS (0 << UCSR0C_USBS0)	   /* Usar 1 bit de stop */
#define CHAR_SIZE (3 << UCSR0C_UCSZ00)	   /* Usar 8 bits de dato de E/S */
#define READY_TO_READ (1 << UCSR0A_RXC0)   /* Dato listo para leer */
#define READY_TO_WRITE (1 << UCSR0A_UDRE0) /* Búfer listo para escribir */

#define MAX_INT_DIGITS 5
#define MAX_LONG_DIGITS 10
#define MAX_DOUBLE_PRECISION 10

#define EN_RX_TX 0x18
#define UART_RXCIE0 7 

        /* activates transmision and reception */
        serial_port->status_control_b = (unsigned char)(EN_RX_TX | (1<<UART_RXCIE0));
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* Estructura de datos overlay del USART del atmega328 */
typedef struct {
	uint8_t status_control_a; /* ucsr0a USART Control and Status A */
	uint8_t status_control_b; /* ucsr0b USART Control and Status B */
	uint8_t status_control_c; /* ucsr0c USART Control and Status C */
	uint8_t _reserved;        /* reserved */
	uint8_t baud_rate_l;      /* ubrr0l baud rate low */;
	uint8_t baud_rate_h;      /* ubrr0h baud rate high */
	uint8_t data_io;          /* udr0 I/O data */
} volatile uart_t;

/* Puntero a la estructura de los registros del periférico */
uart_t *serial_port = (uart_t*)(0xc0);

void serial_init(void)
{
	/* Configurar los registros High y Low con BAUD_PRESCALE */
	serial_port->baud_rate_h = (unsigned char)(BAUD_PRESCALE >> 8);
	serial_port->baud_rate_l = (unsigned char)(BAUD_PRESCALE);

	/* Configurar un frame de 8bits, con un bit de paridad y bit de stop */
	serial_port->status_control_c = CHAR_SIZE | STOP_BITS | PARITY_MODE;

	/* Activar la recepcion y transmicion - ACTIVA interrupcion en RX */
	serial_port->status_control_b = (RX_E | TX_E | (1<<UART_RXCIE0));
}

void serial_put_char(char c)
{
	while (!(serial_port->status_control_a & READY_TO_WRITE))
		;
	serial_port->data_io = c;
}

char serial_get_char(void)
{
	//while (!((serial_port->status_control_a) & (READY_TO_READ)))
	//	;
	//return (serial_port->data_io);
	return rx_char;
}

void serial_put_str(char *str)
{
	while (*str != '\0') {
		serial_put_char(*str);
		str++;
	}
}

char serial_get_str(char * buffer, int max_string_length)
{
	int i = 0;
	char c;

	/*
	Cuidado, el siguiente es un mensaje valido:
	'hola mundo \n \r que tal?'
	*/
	do {
		c = serial_get_char();
		buffer[i] = c;
		i++;
	} while (c != '\0' && i < (max_string_length - 1));

	/*Se añade el carácter nulo al final del string para marcar su terminación*/
	buffer[i] = '\0'; 
	return 1;
}

void serial_put_long_int (long int value, int num_digits)
{
	char buffer[MAX_LONG_DIGITS] = {0};
	int i = 0;

	if (value == 0) {
		return serial_put_char('0');
	} else if (value < 0) {
		serial_put_char('-');
		value = -value; // Usar valor absoluto
	}

	while (value != 0) {
		buffer[i] = value % 10;
		value /= 10;
		i++;
	}

	if (num_digits < 1 || num_digits > MAX_LONG_DIGITS)
		num_digits = i;

	for (int j = num_digits - 1; j > -1; j--)
		serial_put_char(48 + buffer[j]);
}

void serial_put_int (int value, int num_digits)
{
	if (num_digits > MAX_INT_DIGITS)
		num_digits = MAX_INT_DIGITS;

	return serial_put_long_int(value, num_digits);
}

void serial_put_double (double value, int int_digits, int frac_digits)
{
	long int int_value = (long int) value;
	double frac_value = value - int_value;
	int frac_digit = 0;

	serial_put_long_int(int_value, int_digits);
	serial_put_char('.');

	if (frac_digits > MAX_DOUBLE_PRECISION)
		frac_digits = MAX_DOUBLE_PRECISION;
	else if (frac_digits < 1)
		frac_digits = 1;

	if (frac_value < 0)
		frac_value = -frac_value;

	for (int i = 0; i < frac_digits; i++) {
		frac_value *= 10;
		frac_digit = (int) frac_value;
		serial_put_char(48 + (char) frac_value);
		frac_value -= frac_digit;
	}
}

uint8_t rx_data = 0;
char rx_char = 0;

/*
 * RX interrupt service rutine
 */
ISR(USART_RX_vect){
        rx_char = UDR0;             /* read UART register into value */
	rx_data = 1;
}

void serial_cli_rx_data(void)
{
	rx_data = 0;
}

int serial_rx_data(void)
{
	return rx_data;
}
