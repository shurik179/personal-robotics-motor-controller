#ifndef _PRMC_PINS_H
#define _PRMC_PINS_H


//pins
#define PIN_PRMC_SDA     0
#define PIN_PRMC_SCL     1
#define PIN_MOTOR1_DIR   20
#define PIN_MOTOR1_PWM   28
#define PIN_MOTOR2_DIR   2
#define PIN_MOTOR2_PWM   10
#define PIN_JMP0         12
#define PIN_JMP1         13
#define PIN_JMP2         14
#define PIN_ENC1_A       19
#define PIN_ENC1_B       18
#define PIN_ENC2_A       7
#define PIN_ENC2_B       6
#define PIN_NEOPIXEL     3
#define PIN_ERROR1       24
#define PIN_ERROR2       5
#define PIN_DISABLE1     27
#define PIN_DISABLE2     11

//function declaration to initialize pins
void pins_init();
#endif
