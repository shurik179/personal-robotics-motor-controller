#ifndef _MD_I2C_H
#define _MD_I2C_H
#include <Arduino.h>
#define  MAX_TRANSMIT_SIZE 8 //maximal number of bytes the master can request

void i2c_begin();
void i2c_request_event();
void i2c_receive_event(int bytes_received);


#endif
