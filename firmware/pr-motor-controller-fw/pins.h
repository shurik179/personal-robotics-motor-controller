#ifndef _PRMC_PINS_H
#define _PRMC_PINS_H


//pins
#define PIN_PRMC_SDA       0
#define PIN_PRMC_SCL       1
#define PIN_MOTOR1_DIR   27
#define PIN_MOTOR1_PWM   28
#define PIN_MOTOR2_DIR   2
#define PIN_MOTOR2_PWM   3
#define PIN_JMP1         5
#define PIN_JMP2         4
#define PIN_ENC1_A       16
#define PIN_ENC1_B       15
#define PIN_ENC2_A       14
#define PIN_ENC2_B       13
#define PIN_NEOPIXEL     18
#define PIN_ERROR1       17
#define PIN_ERROR2       12
#define PIN_DISABLE      26

//function declaration to initialize pins
void pins_init();
#endif
