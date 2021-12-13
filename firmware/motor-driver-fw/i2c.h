#ifndef _MD_I2C_H
#define _MD_I2C_H
#include <Arduino.h>
#define  MAX_TRANSMIT_SIZE 16 //maximal number of bytes the master can request
extern volatile uint8_t requestAddress;

void i2cSlaveBegin(uint8_t address);
void i2cSlaveRequestEvent();
void i2cSlaveReceiveEvent(int bytesReceived);


#endif
