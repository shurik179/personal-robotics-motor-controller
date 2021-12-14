#ifndef _MD_MOTORS_H
#define _MD_MOTORS_H

void motors_init();
void motors_on_off(uint8_t status);
void motors_set_speeds();
void motors_set_raw(int16_t power1, int16_t power2);
void encoders_reset();
void compute_speed();
void motors_pid_update();

#endif
