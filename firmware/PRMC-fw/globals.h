#ifndef _PRMC_GLOBAL_H
#define _PRMC_GLOBAL_H



/* various global variables, defines, etc */


#define FW_VERSION_MAJOR 2
#define FW_VERSION_MINOR 0

#define DEFAULT_I2C_ADDRESS 0x54

//max motor power
#define MAX_MOTOR_POWER 1000

//Motor PID modes
#define MODE_NOPID 0
#define MODE_PID   1



//Statuses
#define STATUS_ON        0
#define STATUS_M1_OFF    1
#define STATUS_M2_OFF    2

//colors for Neopixel; note that it uses grb color order
#define  RED             0x004000
#define  GREEN           0x400000
#define  BLUE            0x000040

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
#define REG_PID_MODE            1
#define REG_MAX_SPEED           2
#define REG_PID_KP              4
#define REG_PID_TI              6
#define REG_PID_TD              8
#define REG_PID_ILIM            10
#define REG_POWER1              12
#define REG_POWER2              14
#define REG_REVERSE             16
//#define REG_ENC_RESET           17
//Read-only registers
#define REG_FW_VERSION          24
#define REG_WHO_AM_I            26
#define REG_STATUS              27
#define REG_ENCODER1            28
#define REG_ENCODER2            32
#define REG_SPEED1              36
#define REG_SPEED2              38


/* **********************************************************************
 *  pointer/aliases - for direct access to registers. These are forward declarations,
 * the definitions are in regmap.cpp
 * *********************************************************************
 */

extern volatile uint8_t  * motor_enable;
extern volatile uint16_t * motor_maxspeed;
extern volatile uint16_t * motor_Kp;
extern volatile uint16_t * motor_Ti;
extern volatile uint16_t * motor_Td;
extern volatile uint16_t * motor_Ilim;
extern volatile uint8_t  * pid_mode;
extern volatile int16_t  * motor_power; //2-element array
extern volatile uint8_t  * reverse_config; //configurationd ata about motors and encoders - shoudl they be reversed?
//extern volatile byte     * encoder_reset;

/* *********************************************
 *  Read-only registers
 * *********************************************
 */


extern volatile uint8_t  * fw_version; //2-element array
extern volatile uint8_t  * who_am_i;
extern volatile uint8_t  * motor_status;
extern volatile int32_t  * encoder_raw; //2-element array
extern volatile int32_t  * encoder; //2-element array - these are the vlaues after possible reversal
extern volatile int16_t  * speed;   //2-element array

/*
 FLAGS
*/

extern volatile bool flag_enable;
extern volatile bool flag_motor_power;

/* other global variables */
extern volatile bool have_i2c;

/* *********************************************
 *  Function declaration
 * *********************************************
 */

void globals_init();


#endif
