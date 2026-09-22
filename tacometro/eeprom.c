#include "eeprom.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void eeprom_write(uint16_t pos, uint8_t value)
{
    /* Polling */
    while (EECR & (1 << EEPE))
        ;

    EECR = 0;                          /* Borrar bits de control          */
    EEAR = pos;                        /* Direccion a escribir            */
    EEDR = value;                      /* Dato a escribir                 */

    /* Habilitar escritura maestra y escribir */
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

uint8_t eeprom_read(uint16_t pos)
{
    /* Polling */
    while (EECR & (1 << EEPE))
        ;

    EEAR = pos;		/* Direccion a leer */
    /* Iniciar lectura */
    EECR |= (1 << EERE);

    return EEDR;
}
