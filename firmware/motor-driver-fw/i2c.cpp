#include "i2c.h"
#include <Wire.h>
#include "wiring_private.h"
#include "globals.h"
volatile uint8_t requestAddress;
// Flags


void i2cSlaveBegin(uint8_t address){
  Wire.begin(address);
  //pinPeripheral(PIN_WIRE_SDA,PIO_SERCOM);
  //pinPeripheral(PIN_WIRE_SCL,PIO_SERCOM);
  Wire.onRequest(i2cSlaveRequestEvent);
  Wire.onReceive(i2cSlaveReceiveEvent);
}

void i2cSlaveRequestEvent(){
  //put maximum possible number of bytes in the buffer - the master will stop transimssion
  //after reading as many as it needs
  //start at offset requestAddress - the one received from master at last transimssion
  // All reads use REGA
  Wire.write((char *)REGBANK+requestAddress, MAX_TRANSMIT_SIZE);
  //Serial.print("Sent bytes starting at offset "); Serial.println(requestAddress);
}

void i2cSlaveReceiveEvent(int bytesReceived){
    uint8_t reg_address=Wire.read();//get the register offset, always first byte sent
    if ((bytesReceived>1) && (reg_address<RW_REGISTERS)) {
        //this was to write data to register
        for (int i=0; i<bytesReceived-1; i++){
            REGBANK[reg_address+i]=Wire.read();
        }
        switch (reg_address) {
          case REG_ENABLE:
            flag_enable=true;
            break;
          case REG_ENC_RESET:
            flag_enc_reset=true;
            break;
          case REG_POWER1:
          case REG_POWER2:
            flag_motor_power=true;
        }
    } else {
        requestAddress=reg_address; //save for request handler
    }
}
