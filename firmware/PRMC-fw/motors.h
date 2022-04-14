#ifndef _PRMC_MOTORS_H
#define _PRMC_MOTORS_H
#include "globals.h"

#define MOTOR_FREQ 10000 

void motors_init();
void motors_on_off(uint8_t status);
void motors_set_speeds();
void motors_set_raw(int16_t power1, int16_t power2);
void compute_speed();
void motors_pid_update();
//updates latest encoder reading, reversed if necessary
inline void update_encoders(){
    if (*reverse_config & 0x01) {
        encoder[0]= - encoder_raw[0];
    } else {
        encoder[0]=   encoder_raw[0];
    }
    if (*reverse_config & 0x04) {
        encoder[1]= - encoder_raw[1];
    } else {
        encoder[1]=   encoder_raw[1];
    }
}
#endif
