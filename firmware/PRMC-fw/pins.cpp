#include <Arduino.h>
#include "pins.h"

void pins_init(){
    pinMode(PIN_ERROR1, INPUT);
    pinMode(PIN_ERROR2, INPUT);
    pinMode(PIN_JMP0, INPUT_PULLUP);
    pinMode(PIN_JMP1, INPUT_PULLUP);
    pinMode(PIN_JMP2, INPUT_PULLUP);
    pinMode(PIN_DISABLE1, OUTPUT);
    pinMode(PIN_DISABLE2, OUTPUT);
    pinMode(PIN_MOTOR1_DIR, OUTPUT);
    pinMode(PIN_MOTOR1_PWM, OUTPUT);
    pinMode(PIN_MOTOR2_DIR, OUTPUT);
    pinMode(PIN_MOTOR2_PWM, OUTPUT);
    pinMode(PIN_NEOPIXEL, OUTPUT);
}
