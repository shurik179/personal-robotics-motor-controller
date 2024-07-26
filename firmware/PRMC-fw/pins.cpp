#include <Arduino.h>
#include "pins.h"

void pins_init(){
  pinMode(PIN_TX_CTRL, OUTPUT);
  pinMode(PIN_ERROR1, INPUT);
  pinMode(PIN_ERROR2, INPUT);
  pinMode(PIN_ADDR0, INPUT_PULLUP);
  pinMode(PIN_ADDR1, INPUT_PULLUP);
  pinMode(PIN_ENABLE1, OUTPUT);
  pinMode(PIN_ENABLE2, OUTPUT);
  pinMode(PIN_MOTOR1_DIR, OUTPUT);
  pinMode(PIN_MOTOR1_PWM, OUTPUT);
  pinMode(PIN_MOTOR2_DIR, OUTPUT);
  pinMode(PIN_MOTOR2_PWM, OUTPUT);
  pinMode(PIN_NEOPIXEL, OUTPUT);
  //pinMode(PIN_M1_SENSE, INPUT);
  //pinMode(PIN_M2_SENSE, INPUT);
  analogReadResolution(12); //set ADC resolution to 12 bits
}
