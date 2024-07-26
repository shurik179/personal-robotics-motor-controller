//registers
#include "pins.h"
#include "globals.h"
#include "Dynamixel.h"
//code for motors and encoders
#include "motors.h"
#include "neopixel.h"


//uncomment to allow debugging print to Serial.
//#define DEBUG_PRINT

#define SPEED_UPDATE_INTERVAL 40000 //how frequently should speed be computed? in microseconds
                                    //40 ms give frequency of 25 Hz
uint32_t last_speed_update  = micros();
uint32_t current; //in milliamps
DxlPacket instrPacket;
DxlDevice dynamixel;

void setup(){
  pins_init();    //set up pin directions
  globals_init(); // initialize global variables and register bank
  // start Serial1
  Serial1.setRX(PIN_PRMC_RX);
  Serial1.setTX(PIN_PRMC_TX);
  Serial1.begin(BAUDRATE);
  neopixel_init();
  neopixel_set_color(0);
  delay(100);
  motors_init(); //set up motor controller, etc.
  //set ID
  uint8_t id0 = 1- digitalRead(PIN_ADDR0);
  uint8_t id1 = 1- digitalRead(PIN_ADDR1);
  REGBANK[REG_MOTOR_ID] = 1 + id0 + (id1<<1);
  //initialize dynamixel interface 
  dynamixel.begin(&Serial1, BAUDRATE, REGBANK[REG_MOTOR_ID], PIN_TX_CTRL);
  //test only 
  motors_set_raw(500,500);  
  
}

void processPacket(DxlPacket * packet){
  //deal with received packet 
  uint8_t i,addr, num_bytes;
  switch (packet->instr){
    case(DXL_READ_DATA):
      addr = packet->params[0];
      num_bytes = packet->params[1]; //number of bytes to read
      if ((addr+num_bytes)<= REG_SIZE32 * 4 ) {
        dynamixel.sendPacket(num_bytes+2, NO_ERROR, &REGBANK[addr]);//FIXME: NO_ERROR
      }
      break;
    case (DXL_WRITE_DATA):
      addr = packet->params[0];
      num_bytes = packet->length - 3; //number of bytes to write
      if (addr + num_bytes <= RW_REGISTERS) {
        for (i = 0; i < num_bytes; i++) {
          REGBANK[addr+i] = packet->params[i+1];
        }
      } 
      //now, let us trigger some chnages if needed
      if (addr == REG_ENABLE){
        //turn motors on and off as needed
        motors_on_off(REGBANK[REG_ENABLE]);
      }           
      //in all cases, we set motors speed anew, and possibly (re)start the PID controllers
      motors_set_speeds();
      break;  
  }
}

void loop(){
  // check for received serial packets
  dynamixel.receivePacket(&instrPacket);
  if (instrPacket.error != RECEIVE_ERROR){
    // we have successfully received a packet, addressed to us
    processPacket(&instrPacket);
  }  
  //now, let us check if it is time to compute motor speeds and apply PID corrections
  if (micros()-last_speed_update > SPEED_UPDATE_INTERVAL) {
    last_speed_update = micros();
    //Serial.println(last_speed_update);
    compute_speed();
    if (REGBANK[REG_PID_MODE] == MODE_PID) {
        motors_pid_update();
    }
    //also, let us update motor status 
    REGBANK[REG_STATUS] = ( (1-digitalRead(PIN_ERROR2))<<1) |(1 -  digitalRead(PIN_ERROR1));
    neopixel_update();
    //and let us update current reading 
    // max value of ADC (4095) = 3.3v drop on R_prop = (3.3/1500 = 0.0022A = 2200 uA) current through R_prop
    // =  (2200/450 = 4.889) A current through H-bridge 
    current = (4889 * analogRead(PIN_M1_SENSE))/4095 ;
    Serial.print(analogRead(PIN_M1_SENSE));
    reg_save16(REG_CURRENT1, current);
    Serial.print("M1 current (mA): "); Serial.println(current);
    //same for motor2 
    current = (4889 * analogRead(PIN_M2_SENSE))/4095 ;
    //current = analogRead(PIN_M2_SENSE);
    reg_save16(REG_CURRENT2, current);
    Serial.print("M2 current (mA): "); Serial.println(current);
  }

}
