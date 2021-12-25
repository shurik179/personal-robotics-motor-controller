#include <Arduino.h>
#include "globals.h"
#include "pins.h"
#include "motors.h"
#include "neopixel.h"
#include "QuadratureDecoder.h"

int32_t prev_encoder[]={0,0};
uint32_t prev_encoder_update=micros(); //time of last update to revious encoder readign, in microseconds
int32_t enc1_index, enc2_index;



QuadratureDecoder decoder;

void motors_init(){
    analogWriteFreq(MOTOR_FREQ); 
    analogWriteRange(MAX_MOTOR_POWER);
    prev_encoder[0]=0;
    prev_encoder[1]=0;
    speed[0]=0;
    speed[1]=0;
    //disable drivers
    digitalWrite(PIN_DISABLE, HIGH);
    * motor_status = STATUS_M1_OFF|STATUS_M2_OFF;
    * pid_mode = MODE_NOPID;
    neopixel_update();
    //now, initialize encoders
    decoder.init(pio0);
    enc1_index = decoder.addQuadratureEncoder(PIN_ENC1_B, &encoder_raw[0]);
    enc2_index = decoder.addQuadratureEncoder(PIN_ENC2_B, &encoder_raw[1]);
}
void motors_on_off(uint8_t status){
    if (status){
        //enabling motors
        digitalWrite(PIN_DISABLE, LOW);
        * pid_mode = MODE_NOPID;
        motor_power[0]=0;
        motor_power[1]=0;
        * motor_status = STATUS_ON;
        //encoders_reset();
    } else {
        //disabling motors
        digitalWrite(PIN_DISABLE, HIGH);
        * motor_status = STATUS_M1_OFF|STATUS_M2_OFF;

    }
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
    if (power1<0) power1=-power1;
    if (power2<0) power2=-power2;
    digitalWrite(PIN_MOTOR1_DIR, dir1);
    analogWrite(PIN_MOTOR1_PWM, power1);
    digitalWrite(PIN_MOTOR2_DIR, dir2);
    analogWrite(PIN_MOTOR2_PWM, power2);
}
void compute_speed(){
    uint32_t delta=micros()- prev_encoder_update;
    update_encoders();
    int32_t count0=encoder[0];
    int32_t count1=encoder[1];
    speed[0] = ((count0-prev_encoder[0])*1000000.0)/delta;
    speed[1] = ((count1-prev_encoder[1])*1000000.0)/delta;
    Serial.println(speed[0]);
    prev_encoder[0]=count0;
    prev_encoder[1]=count1;
    prev_encoder_update=micros();
}
void motors_pid_update(){
    //FIXME
}
