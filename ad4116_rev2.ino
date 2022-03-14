#include <SPI.h>
#include "AD4116_read.h"

#define DATAOUT 11      // COPI/MOSI
#define DATAIN 12       // CIPO/MISO
#define spiCLOCK 13     // SCK/CLK
#define CHIPSELECT 5  // SS/CS
#define spiMODE3 3



#if defined(ARDUINO_AVR_UNO)
    SPIClass spi;
//ARDUINO MEGA 1256
#elif defined(ARDUINO_AVR_MEGA)
    SPIClass spi;
//ARDUINO MEGA 2560
#elif defined(ARDUINO_AVR_MEGA2560)
    SPIClass spi;
//ARDUINO UNO WIFI
#elif defined(ARDUINO_AVR_UNO_WIFI_DEV_ED)
    SPIClass spi;
//ARDUINO UNO WIFI Rev 2
#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)
    SPIClass spi(DATAIN, spiCLOCK, DATAOUT, CHIPSELECT, spiMODE3);
#endif


void setup()
{
    // Main setting
    Serial.begin(19200);

    pinMode(DATAOUT, OUTPUT);
    pinMode(DATAIN, INPUT);
    pinMode(spiCLOCK, OUTPUT);
    pinMode(CHIPSELECT, OUTPUT);
    digitalWrite(CHIPSELECT, HIGH); // disable device

    spi.begin();
}



void loop()
{

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
    }

    uint16_t ID = AD4116_readID(CHIPSELECT);
    Serial.print("ID: ");
    Serial.println(ID, HEX);

    uint8_t STATUS = AD4116_readStatus(CHIPSELECT);
    Serial.print("Status: ");
    Serial.println(STATUS, BIN);
    Serial.print("Ready: ");
    Serial.println(bitRead(STATUS, 7));

    uint16_t GPOIConfig = AD4116_readGPOIConfig(CHIPSELECT);
    Serial.print("GPOIConfig: ");
    Serial.println(GPOIConfig, BIN);

    uint32_t ChannelOneOffsetRead1 = AD4116_read24(spi, CHIPSELECT, 0b01110000);
    Serial.print("ChannelOneOffsetRead1: ");
    Serial.println(ChannelOneOffsetRead1, BIN);
    
    pinMode(CHIPSELECT,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(spiMODE3);
    
    digitalWrite(CHIPSELECT,LOW); //enable device

    delay(150);

    spi.transfer(0b00110000);
    if (ChannelOneOffsetRead1 == 0b10000000){
        spi.transfer(0b10000001);
        spi.transfer(0b00000000);
        spi.transfer(0b00000000);
    }else {
        spi.transfer(0b10000000);
        spi.transfer(0b00000000);
        spi.transfer(0b00000000);
    }
    
    digitalWrite(CHIPSELECT,HIGH); //disable device


    uint32_t ChannelOneOffsetRead2 = AD4116_read24(spi, CHIPSELECT, 0b01110000);
    Serial.print("ChannelOneOffsetRead2: ");
    Serial.println(ChannelOneOffsetRead2, BIN);

    Serial.println("___");
}



/* Read Chip Address 0x00*/
uint8_t AD4116_readStatus(uint8_t ChipSelectPin)
{
return AD4116_read(spi, ChipSelectPin, 0b01000000);
}

/* Read Chip Address 0x00 Bit 7*/
boolean AD4116_isReady(uint8_t ChipSelectPin){
    uint8_t status = AD4116_readStatus(ChipSelectPin);
    if (bitRead(status,7)){
        return true;
    }
    return false;
}

/* Read Chip Address 0x06*/
uint16_t AD4116_readGPOIConfig(uint8_t ChipSelectPin){
    return AD4116_read16(spi, ChipSelectPin, 0b01000110);
}

/* Read Chip Address 0x07*/
uint16_t AD4116_readID(uint8_t ChipSelectPin)
{
return AD4116_read16(spi, ChipSelectPin, 0b01000111);
}

// uint8_t AD4116_writeChannelSetup(uint8_t ChipSelectPin, uint8_t channel){

//     if (!AD4116_isReady || channel < 0 || channel > 6) {
//         return 1;
//     }
//     return 0;
// }

uint8_t AD4116_writeChannelZeroOffset(uint8_t ChipSelectPin, uint8_t channel, uint32_t offset){

    if (channel < 0 || channel > 6) {
        return 1;
    }
    
    uint8_t comms;

    switch (channel)
    {
        case 0:
        {
            comms= 0b00110000;
            break;
        }
        case 1:
        {
            comms= 0b00110001;
            break;
        }
        case 2:
        {
            comms= 0b00110010;
            break;
        }
        case 3:
        {
            comms= 0b00110011;
            break;
        }
        case 4:
        {
            comms= 0b00110100;
            break;
        }
        case 5:
        {
            comms= 0b00110101;
            break;
        }
        case 6:
        {
            comms= 0b00110110;
            break;
        }
        case 7:
        {
            comms= 0b00110111;
            break;
        }
                                    
    }
    uint8_t data[3];
    data[1] = (offset >> 16) & 0xFF;
    data[2] = (offset >> 8) & 0xFF;
    data[3] = (offset >> 0) & 0xFF;


    //AD4116_write(spi, ChipSelectPin, comms, data);

    return 0;
}

uint8_t AD4116_readx(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(spiMODE3);
    
    digitalWrite(ChipSelectPin,LOW); //enable device

    delay(150);

    spi.transfer(comms);
    uint8_t results = spi.transfer(0x00);
    
    digitalWrite(ChipSelectPin,HIGH); //disable device

    return results;
}


uint16_t AD4116_read16x(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(spiMODE3);
    
    digitalWrite(ChipSelectPin,LOW); //enable device

    delay(150);

    spi.transfer(comms);
    uint16_t results = spi.transfer16(0x00);
    
    digitalWrite(ChipSelectPin,HIGH); //disable device

    return results;
}

uint32_t AD4116_read24x(SPIClass& spi, uint8_t ChipSelectPin, uint8_t comms)
{

    pinMode(ChipSelectPin,OUTPUT);
    
    delay(50);
    
    spi.setBitOrder(MSBFIRST);
    spi.setDataMode(spiMODE3);
    
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