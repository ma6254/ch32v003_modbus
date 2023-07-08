#ifndef BYTES_H
#define BYTES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void pack_big_16(uint8_t *pbuf, uint16_t a);
void pack_big_32(uint8_t *pbuf, uint32_t a);
uint16_t unpack_big_16(uint8_t *pbuf);
uint32_t unpack_big_32(uint8_t *pbuf);
void pack_little_16(uint8_t *pbuf, uint16_t a);
void pack_little_32(uint8_t *pbuf, uint32_t a);
uint16_t unpack_little_16(uint8_t *pbuf);
uint32_t unpack_little_32(uint8_t *pbuf);

#endif /* End of BYTES_H */
