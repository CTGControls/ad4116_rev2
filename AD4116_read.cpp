#include "AD4116_read.h"


uint8_t AD4116_read(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(SPI_MODE3);
    
    digitalWrite(ChipSelectPin,LOW); //enable device

    delay(150);

    spi.transfer(comms);
    uint8_t results = spi.transfer(0x00);
    
    digitalWrite(ChipSelectPin,HIGH); //disable device

    return results;
}


uint16_t AD4116_read16(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(SPI_MODE3);
    
    digitalWrite(ChipSelectPin,LOW); //enable device

    delay(150);

    spi.transfer(comms);
    uint16_t results = SPI.transfer16(0x00);
    
    digitalWrite(ChipSelectPin,HIGH); //disable device

    return results;
}

uint32_t AD4116_read24(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(SPI_MODE3);
    
    digitalWrite(ChipSelectPin,LOW); //enable device

    delay(150);

    spi.transfer(comms);
    uint32_t resultsW1 = spi.transfer(0x00);
    uint32_t resultsW2 = spi.transfer(0x00);
    uint32_t resultsW3 = spi.transfer(0x00);
    
    digitalWrite(ChipSelectPin,HIGH); //disable device

    uint32_t results = resultsW1 + (resultsW2 << 8) + (resultsW3 << 16);
    return results;
}