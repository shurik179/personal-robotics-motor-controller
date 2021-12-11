#ifndef _YOZH_I2C_H
#define _YOZH_I2C_H
#include <Arduino.h>
#define  SLAVE_ADDRESS  0x11  //slave address=17
#define  MAX_TRANSMIT_SIZE 16 //maximal number of bytes the master can request

// allowed values of frequency: 100 000 (regular) or 400 000 (fast mode)
void i2cMasterBegin(int freq);
void i2cMasterWriteByte(uint8_t address, uint8_t regAddress, uint8_t data);
uint8_t i2cMasterReadByte(uint8_t address, uint8_t regAddress);
void i2cMasterReadBytes(uint8_t address, uint8_t regAddress, uint8_t count, uint8_t * dest);
void i2cSlaveBegin();
void i2cSlaveRequestEvent();
void i2cSlaveReceiveEvent(int bytesReceived);


#endif
