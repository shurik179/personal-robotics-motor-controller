#ifndef _MD_GLOBAL_H
#define _MD_GLOBAL_H
#include <Arduino.h>
/* various global variables, defines, etc */

#define DEFAULT_I2C_ADDRESS 0x54

//Motor modes
#define MOTOR_MODE_NOPID 0
#define MOTOR_MODE_PID   1

//Statuses
#define STATUS_OFF       0
#define STATUS_ON        1
#define STATUS_ERROR     2

//colors for Neopixel; note that it uses grb color order
#define  RED             0x00FF00
#define  GREEN           0xFF0000


//pins 
#define PIN_MOTOR1_DIR   27
#define PIN_MOTOR1_PWM   28
#define PIN_MOTOR2_DIR   2
#define PIN_MOTOR2_PWM   3
#define PIN_ENC1_A       16
#define PIN_ENC1_B       15
#define PIN_ENC2_A       14
#define PIN_ENC2_B       13
#define PIN_NEOPIXEL     18
#define PIN_ERROR1       17
#define PIN_ERROR2       12
#define PIN_ENABLE       26

// I2C register bank
#define REG_SIZE32 10 //size of register bank, in 4-byte (32 bit) units
#define RW_REGISTERS 24 //number of RW registers (in bytes)
//this will be defined in regmap.cpp
extern  volatile byte * REGBANK;

/* **********************************************************************
 *  Register definitions
 *  All multibyte registers  use little-endian byte order: thus, if
 * a uint16_t value is stored in registers N, N+1,
 * then register N is low byte, and register N+1 is high byte
 * so the full value is
 * (REGBANK[N+1]<<8)|REGBANK[N]
 * This allows directly addressing them as multibyte variables, e.g.
 * uint16_t * value =(uint16_t *) &REGBANK[N]; //pointer to variable
 * *value=25;
 * Yes, it is a bit hacky
 * *********************************************************************
 */
//R/W registers
#define REG_ENABLE              0
#define REG_MOTOR_MODE          1
#define REG_MAX_SPEED           2
#define REG_PID_KP              4
#define REG_PID_TI              6
#define REG_PID_TD              8
#define REG_PID_ILIM            10
#define REG_POWER1              12
#define REG_POWER2              14
#define REG_ENC_RESET           16
//Read-only registers
#define REG_FW_VERSION          24
#define REG_WHO_AM_I            26
#define REG_STATUS              27
#define REG_ENCODER1            28
#define REG_ENCODER2            30
#define REG_SPEED1              32
#define REG_SPEED2              34


/* **********************************************************************
 *  pointer/aliases - for direct access to registers. These are forward declarations,
 * the definitions are in regmap.cpp
 * *********************************************************************
 */

extern volatile uint8_t  * motorEnable;
extern volatile uint16_t * motorMaxspeed;
extern volatile uint16_t * motorKp;
extern volatile uint16_t * motorTi;
extern volatile uint16_t * motorTd;
extern volatile uint16_t * motorIlim;
extern volatile uint8_t  * motorMode;
extern volatile byte     * encoderReset;
extern volatile int16_t  * motorPower; //2-element array

/* *********************************************
 *  Read-only registers
 * *********************************************
 */


extern volatile uint8_t  * fwVersion; //2-element array
extern volatile uint8_t  * whoAmI;
extern volatile uint8_t  * motorStatus;
extern volatile int32_t  * encoder; //2-element array
extern volatile int16_t  * speed;   //2-element array


#endif
