#include <Arduino.h>
#include "globals.h"
#include "pins.h"
#include "motors.h"
#include "neopixel.h"

int32_t prev_encoder[]={0,0};


void motors_init(){
    analogWriteFreq(18000); //set frequency to 18 kHz
    analogWriteRange(MAX_MOTOR_POWER);
    encoders_reset();
    digitalWrite(PIN_DISABLE, HIGH);
    * motor_status = STATUS_OFF;
    * pid_mode = MODE_NOPID;
    neopixel_update();
}
void motors_on_off(uint8_t status){
    if (status==STATUS_ON){
        //enabling motors
        digitalWrite(PIN_DISABLE, LOW);
        * pid_mode = MODE_NOPID;
        motor_power[0]=0;
        motor_power[1]=0;
        encoders_reset();
    } else {
        //disabling motors
        digitalWrite(PIN_DISABLE, HIGH);
    }
    //in all cases, let us also update status and neopixel color
    * motor_status = status;
    neopixel_update(); //set neopixel color to indicate new status
}
void motors_set_speeds(){
    int16_t power1, power2;
    switch (*pid_mode){
        case MODE_NOPID:
            power1=motor_power[0];
            power2=motor_power[1];
            break;
        case MODE_PID:
            //FIXME - need to activate PID 
            break;
    }
    motors_set_raw(power1, power2);
}
void motors_set_raw(int16_t power1, int16_t power2){
    uint8_t dir1=(power1>=0);
    uint8_t dir2=(power2>=0);
    digitalWrite(PIN_MOTOR1_DIR, dir1);
    analogWrite(PIN_MOTOR1_PWM, power1);
    digitalWrite(PIN_MOTOR2_DIR, dir2);
    analogWrite(PIN_MOTOR2_PWM, power2);
}
void encoders_reset(){
    encoder[0]=0;
    encoder[1]=0;
    prev_encoder[0]=0;
    prev_encoder[1]=0;
    speed[0]=0;
    speed[1]=0;
}
void compute_speed(){
    //FIXME
}
void motors_pid_update(){
    //FIXME
}
