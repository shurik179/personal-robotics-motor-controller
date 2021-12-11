#include "PID.h"

void PIDcontroller::configure(volatile  float p, volatile  float i, volatile  float d){
  Kp=p;
  Ki=i;
  Kd=d;
  Ilim=0;
  _is_active = false;
}
void PIDcontroller::configure(volatile  float p, volatile  float i, volatile  float d, volatile float lim){
  Kp=p;
  Ki=i;
  Kd=d;
  Ilim=lim;
  _is_active = false;
}
void PIDcontroller::configure(volatile float * pidCoef){
  Kp=pidCoef[0];
  Ki=pidCoef[1];
  Kd=pidCoef[2];
  Ilim=pidCoef[3];
  _is_active = false;
  //Serial.print("PID configured: ");
  //Serial.print(Kp,5);Serial.print(Ki,5);Serial.print(Kd,5);Serial.println(Ilim,5);
}
void PIDcontroller::reset(){
  target=0;
  prevError=0;
  intError=0;
  lastReset=micros();
  _is_active = false;
}
void PIDcontroller::setTarget(volatile float t){
  target=t;
  prevError=0;
  intError=0;
  lastReset=micros();
  //Serial.print("PID target: ");Serial.println(t);
  _is_active = true;
}
bool PIDcontroller::is_active() {
  return _is_active;
}
float PIDcontroller::update(float reading){
  float error,dError;
  //double controlOutput;
  float deltat;
  uint32_t now=micros();

  if (! _is_active) return 0.0;

  error=target-reading;

  if (now-lastReset>300000){
      //reset was more than 0.3 s ago - can use integral and derivative terms
      //otherwise, ignore P and D terms - too much volatility
      deltat=(now-lastUpdate)*0.000001f; //time in seconds
      intError+=error*deltat;
      if (Ilim>0){ //limit the integral error, to prevent integral windup
        if (intError>Ilim) intError=Ilim;
        else if (intError<-Ilim) intError=-Ilim;
      }
      dError=(error-prevError)/deltat;
  }
  lastUpdate=now;
  prevError=error;
  // now, compute the feedback
  float output=Kp*error+Ki*intError+Kd*dError;
  //Serial.print("Kp: "); Serial.println(Kp,4);
  //Serial.print("Target: "); Serial.println(target,4);
  //Serial.print("Error: "); Serial.println(error,4);
  //Serial.print("Deltat: "); Serial.println(deltat,8);
  //Serial.print("Int Error: "); Serial.println(intError,4);
  //Serial.print("D Error: "); Serial.println(dError,4);
  //Serial.print("Kp*Error: "); Serial.println(Kp*error,4);
  //Serial.print("PID output: "); Serial.println(output,4);
  //Serial.print("PID returned: "); Serial.println(controlOutput,4);
  return output;
}
