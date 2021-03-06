#include <stdint.h>

uint8_t eeprom_write (uint8_t *src, const uint32_t offset, const uint32_t size);
uint8_t eeprom_read  (uint8_t *dest, const uint32_t offset, const uint32_t size);
uint8_t erasePage (const uint8_t p);
uint8_t eraseAllPages (void );
