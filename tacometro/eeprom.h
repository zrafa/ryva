#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/**
 * Escribe un byte en la EEPROM.
 * @param pos   Dirección (0 .. 1023 en ATmega328P)
 * @param value Byte a escribir
 */
void eeprom_write(uint16_t pos, uint8_t value);

/**
 * Lee un byte de la EEPROM.
 * @param pos Dirección (0 .. 1023 en ATmega328P)
 * @return El byte almacenado en esa posición
 */
uint8_t eeprom_read(uint16_t pos);

#endif /* EEPROM_H */
