#include "Arduino.h"
#include "Dynamixel.h"

#define sendData(args)      (serialPort->write((uint8_t)args))  // Write Over Serial
#define availableData()     (serialPort->available())           // Check Serial Data Available
#define readData()          (serialPort->read())                // Read Serial Data

void DxlDevice::begin(HardwareSerial *sPort,uint32_t bRate, uint8_t deviceID, uint8_t pin) {	
	serialPort = sPort;
  baudRate = bRate;
	txDelay = (20*1000000)/bRate; //time in microseconds to send 20 bits (2 bytes, including start and stop bits)
  ID = deviceID;
  ctrlPin = pin;
  pinMode(ctrlPin, OUTPUT);
  digitalWrite(ctrlPin, LOW); //disable TX 
}


// receive instrction packet packet
uint8_t DxlDevice::receivePacket(DxlPacket * packet) {
  unsigned short timeCounter = 0;
  uint8_t i, incomingByte, sum, checksum;
  bool startMarkerFound = false;

  packet->error = RECEIVE_ERROR; // by default, assume that we haven't received a valid message
  sum = 0;  
  
  while (!startMarkerFound && (availableData() >=6)) { //shortest instruction packet is 6 bytes
    incomingByte = readData();
    startMarkerFound = ( (incomingByte == DXL_START) && (serialPort->peek() == DXL_START) );
  }
  if (!startMarkerFound ) return(RECEIVE_ERROR); //got close to the end of receive buffer and found no start marker
  //if we are here, we did find the start marker 
  readData();//skip second 0xFF
  //next, check ID 
  packet->ID = readData();
  if  ( (packet->ID != ID) && (packet->ID != DXL_BROADCAST) ) return(RECEIVE_ERROR);
  //get length 
  uint8_t length = readData();
  packet->length = length;
  //wait until we have enough data in receive buffer 
  while( (availableData() <  length) && (timeCounter < TIME_OUT) ){
    timeCounter++;
    delayMicroseconds(TIME_COUNTER_DELAY);
  }
  delayMicroseconds(TIME_COUNTER_DELAY*2);
  if (availableData() < length) return(RECEIVE_ERROR); //timeout - never got complete packet 
  packet->instr = readData();
  sum = packet->length + packet->ID;
  for (i =0; i<length-2; i++){
    incomingByte = readData();
    sum+=incomingByte; 
    packet->params[i] = incomingByte;
  }
  checksum = readData();
  packet->checksum = checksum;
  if (checksum !=(uint8_t) ~sum ){
    packet->error = CHECKSUM_ERROR;
  } else {
    packet->error = NO_ERROR;
  }
  #ifdef DXL_DEBUG        
        Serial.print("Incoming packet checksum: ");Serial.println(checksum, HEX);
        Serial.print("Expected checksum: ");Serial.println((uint8_t)~sum, HEX);
  #endif         
  return (packet->error);
}



//sends status packet
// length is number of parameters +2 
void DxlDevice::sendPacket(uint8_t length, uint8_t error, uint8_t * params) {
  uint8_t checksum;
  uint8_t i;
  //enable Tx mode
  digitalWrite(ctrlPin, HIGH);
  delayMicroseconds(TIME_COUNTER_DELAY);
  //
  //compute checksum 
  checksum = ID +length + error; 
  for (i=0; i<length-2; i++) {
    checksum += params[i];
  }
  checksum = ~checksum;
  sendData(DXL_START);
  sendData(DXL_START);
  sendData(ID);
  sendData(length);
  sendData(error);
  for (i=0; i<length-2; i++) {
    sendData(params[i]);
  }
  sendData(checksum);
  serialPort->flush();
  delayMicroseconds(txDelay); //flush() returns after the last byte has been passed from FIFO to 
                              //the hardware peripheral, but we still need to wait while it physically transmits
  digitalWrite(ctrlPin, LOW);
}
