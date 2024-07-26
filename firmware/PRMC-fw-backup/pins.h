#ifndef _PRMC_PINS_H
#define _PRMC_PINS_H


//pins
#define PIN_PRMC_TX     0
#define PIN_PRMC_RX     1
#define PIN_MOTOR1_DIR   25
#define PIN_MOTOR1_PWM   26
#define PIN_MOTOR2_DIR   5
#define PIN_MOTOR2_PWM   10
#define PIN_ADDR0        23
#define PIN_ADDR1        22
#define PIN_ENC1_A       19
#define PIN_ENC1_B       18
#define PIN_ENC2_A       7
#define PIN_ENC2_B       6
#define PIN_NEOPIXEL     4
#define PIN_ERROR1       20
#define PIN_ERROR2       3
#define PIN_ENABLE1      24
#define PIN_ENABLE2      11

//function declaration to initialize pins
void pins_init();
#endif
