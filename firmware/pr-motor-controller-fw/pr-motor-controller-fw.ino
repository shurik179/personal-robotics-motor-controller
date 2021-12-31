//#include "wiring_private.h"
//registers
#include "pins.h"
#include "globals.h"
#include "i2c.h"
//code for motors, encoders, and servos
#include "motors.h"
#include "neopixel.h"


//uncomment to allow debugging print to Serial.
#define DEBUG_PRINT

#define SPEED_UPDATE_INTERVAL 40000 //how frequently should speed be computed? in microseconds
                                    //40 ms give frequency of 25 Hz
uint32_t last_speed_update  = micros();



void setup(){
    pins_init();    //set up pin directions
    globals_init(); // initialize global variables and register bank
    i2c_begin();    //start i2c bus on Wire, as a slave
    neopixel_init();
    motors_init(); //set up motor controller, etc. Initially it shoudl be in disabled state, with pid mode PID_OFF

    //initRegmap();
    //Serial.println("Setup ends");
}

void loop(){
    //first, check if one of motor drivers has error condition
    uint8_t new_status = (digitalRead(PIN_ERROR2)<<1) | digitalRead(PIN_ERROR1);
    //Serial.println(new_status);
    if (new_status != *motor_status){
        //update status and neopixel
        * motor_status = new_status;
        neopixel_update();
    }
    //now, check if we got enable/disable commands
    if (flag_enable) {
        //we got command from host to enable or disable motors
        flag_enable = false; //unset flag
        motors_on_off( * motor_enable);
    }

    //finally, check if we had a new speed setting for motors
    if (flag_motor_power) {
        flag_motor_power = false;
        motors_set_speeds();
    }
    //now, let su check if it is time to compute motor speeds and apply PID corrections
    if (micros()-last_speed_update > SPEED_UPDATE_INTERVAL) {
        last_speed_update = micros();
        //Serial.println(last_speed_update);
        compute_speed();
        if (*pid_mode == MODE_PID) {
            motors_pid_update();
        }
    }

}
