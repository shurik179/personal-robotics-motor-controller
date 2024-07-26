#ifndef Dynamixel_h
#define Dynamixel_h
/*
 * Library for Dynamixel device
*/

//#define DXL_DEBUG //to enable debugging messages to Serial. Assumes that Serial has been previously started

#define DXL_START      0xFF //header
#define DXL_BROADCAST  0xFE 

// Instruction Set ///////////////////////////////////////////////////////////////
#define DXL_PING              1
#define DXL_READ_DATA         2
#define DXL_WRITE_DATA        3
#define DXL_REG_WRITE         4
#define DXL_ACTION            5
#define DXL_RESET             6
#define DXL_SYNC_WRITE        131

//errors
#define NO_ERROR              0x00
#define RECEIVE_ERROR			    0x80 //this is added - not part of dynamixel protocol
#define INSTRUCTION_ERROR			0x40
#define OVERLOAD_ERROR				0x20
#define CHECKSUM_ERROR				0x10
#define RANGE_ERROR					  0x08
#define OVERHEATING_ERROR			0x04
#define ANGLE_LIMIT_ERROR	    0x02	
#define INPUT_VOLTAGE_ERROR	  0x01

//timing 
#define TIME_COUNTER_DELAY    600 //microseconds - waiting period 
#define TIME_OUT              50  //maximal number of repeats of delay 

#define MAX_PARAMS   16 //maximal numner of parameters in an instruction packet 

#define BAUDRATE 76800

#include <HardwareSerial.h>
// structure to hold a  packet, see https://emanual.robotis.com/docs/en/dxl/protocol1/
// can be used both for status packet (in which case field instr is not used)
// and instruction packet. For instruction packet, the field error, which is not part of 
// dynamixle packet specs, is used to indicate error during receipt (timeout or checksum error)
struct DxlPacket {
  uint8_t ID;
  uint8_t length;
  uint8_t instr;
  uint8_t params[MAX_PARAMS];
  uint8_t checksum;
  uint8_t error;
};

class DxlDevice {
  private: 
    HardwareSerial *serialPort;
    uint8_t baudRate;
    uint32_t txDelay;
    uint8_t ID;
    uint8_t ctrlPin;

  public: 
    void begin(HardwareSerial *sPort, uint32_t bRate, uint8_t deviceID, uint8_t pin);
    uint8_t receivePacket(DxlPacket * packet);
    void sendPacket(uint8_t length, uint8_t error, uint8_t *params);
};



#endif
