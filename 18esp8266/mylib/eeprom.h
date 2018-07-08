#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>

extern void eeprom_init(void);
extern void eeprom_write_byte(uint8_t daddr, uint8_t waddr, uint8_t data);
extern uint8_t eeprom_read_byte(uint8_t daddr, uint8_t waddr);

#endif

