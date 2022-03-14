#ifndef AD4116READ_H
#define AD4116READ_H
#include <Arduino.h>
#include <SPI.h>

    uint8_t AD4116_read(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms);
    uint16_t AD4116_read16(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms);
    uint32_t AD4116_read24(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms);

#endif