#include <Arduino.h>
#include "globals.h"
//initialize/allcate all global variables
//FIXME

//create arrays and aliases
// main register bank
int32_t REG32[REG_SIZE32];
//cast  as byte array
byte * REGBANK = (byte *)REG32;


//create aliases for registers

//volatile uint16_t * motor_maxspeed   =(uint16_t *) &REGBANK[REG_MAX_SPEED];
volatile int16_t  * motor_power      =(int16_t *)  &REGBANK[REG_POWER1]; //2-element array
volatile uint8_t  * reverse_config   =(uint8_t *)  &REGBANK[REG_REVERSE];
//volatile byte     * encoder_reset    = &REGBANK[REG_ENC_RESET];

//Read-only registers
volatile int32_t  * encoder_raw = new int32_t[2];     //raw readigns from decoder library, before reversal
int32_t * encoder = new int32_t[2];
int16_t * speed = new int16_t[2];



//function for initialization registers
void globals_init(){
    REGBANK[REG_PID_MODE] = MODE_NOPID;
    REGBANK[REG_FW_VERSION]=FW_VERSION_MINOR;
    REGBANK[REG_FW_VERSION+1]=FW_VERSION_MAJOR;
}
