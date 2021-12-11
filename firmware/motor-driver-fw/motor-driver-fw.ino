//#include "wiring_private.h"
//registers
#include "globals.h"
#include "i2c.h"
//code for motors, encoders, and servos
#include "motors.h"
#include "neopixel.h"

#define FW_VERSION_MAJOR 2
#define FW_VERSION_MINOR 1
//uncomment to allow debugging print to Serial.
#define DEBUG_PRINT

uint32_t loopCount=0;

void setup(){
    //i2cMasterBegin(100000); //start I2C bus on Wire1 as master, in regular mode (100 kHz)
    i2cSlaveBegin();        //start i2c bus on Wire, as a slave
    initRegmap();
    *whoAmI=WHO_AM_I;
    fwVersion[0]=FW_VERSION_MINOR;
    fwVersion[1]=FW_VERSION_MAJOR;
#ifdef DEBUG_PRINT
        Serial.begin(9600);
        /*while(!Serial){
          delay(10);
        }*/
        delay(1000);
        Serial.println("Yozh firmware started");
        delay(1000);
#endif
    for (int i=0; i<2; i++){
        servoPosition[i] = 1500;
    }
    *motorMode = MOTOR_MODE_POWER;
    setupTimers();
    setupMotorPins();
    setServos();//FIXME
    *linearrayConfig = 0;
    //*imuStatus = IMU_OFF;
    *neopixelBrightness=64;
    neopixelColors[1]=150;
    neopixelColors[4]=150;
    pixelBegin();
    pixelUpdate();
    Serial.println("Setup ends");
}

void loop(){
    loopCount++;
    //High priority: done every cycle
    //First, update configuration/motors/servos
    if (isSet(FLAG_MOTOR_CONFIG)){
        clearFlag(FLAG_MOTOR_CONFIG);
        #ifdef DEBUG_PRINT
            Serial.println("Updating motor configuration");
        #endif
        //FIXME
    }
    if (isSet(FLAG_SERVO)){
        clearFlag(FLAG_SERVO);//unset the servo flag bit
        #ifdef DEBUG_PRINT
        Serial.print("setting servo to new positions: ");
        Serial.print(servoPosition[0]); Serial.println(" ");
        #endif
        setServos();
    }
    if (isSet(FLAG_ENC_RESET)) {
        clearFlag(FLAG_ENC_RESET);
        #ifdef DEBUG_PRINT
        Serial.println("Resetting encoder(s)");
        #endif
        resetEncoders();
    }
    if (isSet(FLAG_MOTOR_MODE)||isSet(FLAG_MOTOR_POWER)){
        clearFlag(FLAG_MOTOR_MODE);
        clearFlag(FLAG_MOTOR_POWER);
        #ifdef DEBUG_PRINT
            Serial.println("Updating motor mode/power  configuration");
            Serial.print("Motor mode: "); Serial.print(*motorMode);
            Serial.print(" powerL: "); Serial.print(motorPower[0]);
            Serial.print(" powerR: "); Serial.println(motorPower[1]);
        #endif
        setMotors();
    }
    if (isSet(FLAG_LINEARRAY_CONFIG)){
        clearFlag(FLAG_LINEARRAY_CONFIG);
        if (*linearrayConfig) {
            enableLineArray();
        } else {
            disableLineArray();
        }
    }
    if (isSet(FLAG_NEOPIXEL_CONFIG)){
        clearFlag(FLAG_NEOPIXEL_CONFIG);
        pixelUpdateConfig();
    }
    if (isSet(FLAG_NEOPIXEL)){
        clearFlag(FLAG_NEOPIXEL);
        pixelUpdate();
    }
    //now, update readings of sensors etc
    updateVsense();
    if (*linearrayConfig) {
        updateLineArray();
    }
    //update motor speed reading and pid speed adjustment
    updateSpeed();
}
