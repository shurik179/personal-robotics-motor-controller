#include <Arduino.h>
#include "globals.h"
#include "pins.h"
#include "motors.h"
#include "neopixel.h"
#include "QuadratureDecoder.h"
#include "pid.h"

int32_t prev_encoder[]={0,0};
uint32_t prev_encoder_update=micros(); //time of last update to revious encoder readign, in microseconds
int32_t enc1_index, enc2_index;

PIDcontroller SpeedController1, SpeedController2;



QuadratureDecoder decoder;

void motors_init(){
    analogWriteFreq(MOTOR_FREQ);
    analogWriteRange(MAX_MOTOR_POWER);
    prev_encoder[0]=0;
    prev_encoder[1]=0;
    speed[0]=0;
    speed[1]=0;
    SpeedController1.reset();
    SpeedController2.reset();
    //disable drivers
    digitalWrite(PIN_DISABLE1, HIGH);
    digitalWrite(PIN_DISABLE2, HIGH);
    * motor_status = STATUS_M1_OFF|STATUS_M2_OFF;
    * pid_mode = MODE_NOPID;
    neopixel_update();
    //now, initialize encoders
    decoder.init(pio0);
    enc1_index = decoder.addQuadratureEncoder(PIN_ENC1_B, &encoder_raw[0]);
    enc2_index = decoder.addQuadratureEncoder(PIN_ENC2_B, &encoder_raw[1]);
}
void motors_on_off(uint8_t status){
    bool m1_status = status & 0x01;
    bool m2_status = status & 0x02;

    if (m1_status){
        //enabling motor1
        digitalWrite(PIN_DISABLE1, LOW);
        motor_power[0]=0;
        * motor_status &= 0b11111110; //set bit 0 to 0
    } else {
        //disabling motor1
        digitalWrite(PIN_DISABLE1, HIGH);
        * motor_status |= 0x01;   //set bit 0 to 1
    }
    if (m2_status){
        //enabling motor1
        digitalWrite(PIN_DISABLE2, LOW);
        motor_power[1]=0;
        * motor_status &= 0b11111101; //set bit 1 to 0
    } else {
        //disabling motor1
        digitalWrite(PIN_DISABLE2, HIGH);
        * motor_status |= 0x02;   //set bit 1 to 1
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
            //set up the PID controllers for left and right motor
            //each of them will take  as input speed (in ticks/s) and outputs a value between -1..1 - it will be later multiplied by MAX_MOTOR_POWER
            float Kp= (*motor_Kp)*0.0000001; //10^7
            float Ti=(*motor_Ti)*0.001; //time constant for integral term
            float Td=(*motor_Td)*0.001; //time constant for derivative term
            float Ilim = (*motor_Ilim); //integral limit, in ticks
            SpeedController1.configure(Kp,Kp/Ti,Kp*Td,Ilim); //Ki=Kp/Ti, Kd=Kp*Td
            SpeedController1.setTarget( (float)motor_power[0]* (*motor_maxspeed)/MAX_MOTOR_POWER); //converting the value on-500...500 scale to ticks/s
            SpeedController2.configure(Kp,Kp/Ti,Kp*Td,Ilim);
            SpeedController2.setTarget( (float)motor_power[1]* (*motor_maxspeed)/MAX_MOTOR_POWER); //converting the value on-500...500 scale to ticks/s
            //set initial power, as 0th approximation
            power1=motor_power[0];
            power2=motor_power[1];
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
    speed[0] = ((count0-prev_encoder[0])*1000000.0)/delta; //FIXME
    speed[1] = ((count1-prev_encoder[1])*1000000.0)/delta;
    Serial.println(speed[0]);
    prev_encoder[0]=count0;
    prev_encoder[1]=count1;
    prev_encoder_update=micros();
}
void motors_pid_update(){
    if (*pid_mode == MODE_NOPID) return;
    int16_t power1 = motor_power[0] + (int16_t) (MAX_MOTOR_POWER*SpeedController1.update((float)speed[0]));
    int16_t power2 = motor_power[1] + (int16_t) (MAX_MOTOR_POWER*SpeedController2.update((float)speed[1]));
    //Serial.print(speed[0]);Serial.print("     "); Serial.println(powerL);
    if (power1>MAX_MOTOR_POWER) power1=MAX_MOTOR_POWER;
    else if (power1<-MAX_MOTOR_POWER) power1=-MAX_MOTOR_POWER;

    if (power2>MAX_MOTOR_POWER) power2=MAX_MOTOR_POWER;
    else if (power2<-MAX_MOTOR_POWER) power2=-MAX_MOTOR_POWER;

    motors_set_raw(power1, power2);
}
