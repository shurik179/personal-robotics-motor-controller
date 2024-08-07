#ifndef _PRMC_GLOBAL_H
#define _PRMC_GLOBAL_H



/* various global variables, defines, etc */


#define FW_VERSION_MAJOR 3
#define FW_VERSION_MINOR 0

//max motor power
#define MAX_MOTOR_POWER 1000

//Motor PID modes
#define MODE_NOPID 0
#define MODE_PID   1



//ENABLE/STATUS bitmasks
#define STATUS_M1        1
#define STATUS_M2        2

#define ENABLE_M1        1
#define ENABLE_M2        2

// register bank
#define REG_SIZE32 10 //size of register bank, in 4-byte (32 bit) units
#define RW_REGISTERS 18 //number of RW registers (in bytes)
//this will be defined in globals.cpp
extern  byte * REGBANK;

/* **********************************************************************
 *  Register definitions
 *  All multibyte registers  use little-endian byte order: thus, if
 * a int16_t value is stored in registers N, N+1,
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
#define REG_REVERSE             1
#define REG_PID_MODE            2
#define REG_MAX_SPEED           4
#define REG_PID_KP              6
#define REG_PID_TI              8
#define REG_PID_TD              10
#define REG_PID_ILIM            12
#define REG_POWER1              14
#define REG_POWER2              16
//Read-only registers
#define REG_MOTOR_ID            20
#define REG_FW_VERSION          21
#define REG_STATUS              23
#define REG_CURRENT1            24
#define REG_CURRENT2            26
#define REG_ENCODER1            28
#define REG_ENCODER2            32
#define REG_SPEED1              36
#define REG_SPEED2              38


/* **********************************************************************
 *  pointer/aliases - for direct access to registers. These are forward declarations,
 * the definitions are in globals.cpp
 * *********************************************************************
 */

//extern volatile uint16_t * motor_maxspeed;
extern volatile int16_t  * motor_power; //2-element array
extern volatile uint8_t  * reverse_config; //configurationd ata about motors and encoders - shoudl they be reversed?
//extern volatile byte     * encoder_reset;

/* *********************************************
 *  Other global variables 
 * *********************************************
 */

extern volatile int32_t  * encoder_raw; //2-element array
extern int32_t  * encoder; //2-element array - these are the vlaues after possible reversal
extern int16_t  * speed;   //2-element array


/* other global variables */



/* *********************************************
 *  Function declaration
 * *********************************************
 */

void globals_init();

inline void reg_save16(uint8_t address, int16_t value){
  REGBANK[address] = value & 0xFF; //low byte
  REGBANK[address+1] = (value>>8) & 0xFF; //high byte
}

inline void reg_save32(uint8_t address, int32_t value){
  REGBANK[address] = value & 0xFF; //low byte
  REGBANK[address+1] = (value>>8) & 0xFF; 
  REGBANK[address+2] = (value>>16) & 0xFF; 
  REGBANK[address+3] = (value>>24) & 0xFF; //high byte
}

inline int16_t reg_read16(uint8_t address) {
  return(REGBANK[address]| (REGBANK[address+1]<<8));
}
#endif
