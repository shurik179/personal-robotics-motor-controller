#include <Arduino.h>
#include "globals.h"
//initialize/allcate all global variables
//FIXME

//create arrays and aliases
// main register bank
volatile int32_t REG32[REG_SIZE32];
//cast  as byte array
volatile byte * REGBANK = (byte *)REG32;


//create aliases for registers

volatile uint8_t  * motor_enable     =(uint8_t *)  &REGBANK[REG_ENABLE];
volatile uint16_t * motor_maxspeed   =(uint16_t *) &REGBANK[REG_MAX_SPEED];
volatile uint16_t * motor_Kp         =(uint16_t *) &REGBANK[REG_PID_KP];
volatile uint16_t * motor_Ti         =(uint16_t *) &REGBANK[REG_PID_TI];
volatile uint16_t * motor_Td         =(uint16_t *) &REGBANK[REG_PID_TD];
volatile uint16_t * motor_Ilim       =(uint16_t *) &REGBANK[REG_PID_ILIM];
volatile uint8_t  * pid_mode         =(uint8_t *)  &REGBANK[REG_PID_MODE];
volatile int16_t  * motor_power      =(int16_t *)  &REGBANK[REG_POWER1]; //2-element array
volatile uint8_t  * reverse_config   =(uint8_t *)  &REGBANK[REG_REVERSE]; 
//volatile byte     * encoder_reset    = &REGBANK[REG_ENC_RESET];

//Read-only registers
volatile uint8_t  * fw_version        = (uint8_t *)&REGBANK[REG_FW_VERSION];//2-element array
volatile uint8_t  * who_am_i          = (uint8_t *)&REGBANK[REG_WHO_AM_I];
volatile uint8_t  * motor_status      = (uint8_t *)&REGBANK[REG_STATUS];
volatile int32_t  * encoder           = (int32_t *)&REGBANK[REG_ENCODER1]; //2-element array
volatile int16_t  * speed             = (int16_t *)&REGBANK[REG_SPEED1];   //2-element array
volatile int32_t *  encoder_raw = new int32_t[2];     //raw readigns from decoder library, before reversal
//flags
//volatile bool flag_enc_reset=false;
volatile bool flag_enable=false;
volatile bool flag_motor_power=false;





//function for initialization registers
void globals_init(){
    *who_am_i = DEFAULT_I2C_ADDRESS;
    *motor_enable = 0;
    *motor_status = STATUS_M1_OFF|STATUS_M2_OFF;
    *pid_mode     = MODE_NOPID;
}
