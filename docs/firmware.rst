.. _firmware:
**************************
Firmware and register map
**************************

Firmware
========

The firmware for the motor controller is written in Arduino IDE using
Earl Philhower's `RP2040 core <https://github.com/earlephilhower/arduino-pico>`__.

If you want to modify it, you can find source files in `firmware` folder in |github|.


I2C communication
=================

The controller is controlled via I2C bus. It supports speeds up to 400kHz.
Default I2C address is `0x54`, but it can be changed by closing the solder
bridge jumpers on the bottom of the board (see `specs`).

The following table lists all controller I2C registers.

.. csv-table:: Register map
   :widths: 10 10 30 10 50
   :header:   "register(s)","R/W","Name","data type","description"

   0,R/W,REG_ENABLE,byte,"write 1 to enable motor drivers, 0 to disable"
   1,R/W,REG_PID_MODE,byte,"0: normal control (incl. brake), PID off; 1: PID on (individual motors)"
   2-3,R/W,REG_MAX_SPEED,uint16,Max motor speed in encoder ticks/s. Required for PID
   4-5,R/W,REG_PID_KP,uint16,Kp*10^7
   6-7,R/W,REG_PID_TI,uint16,Ti*1000
   8-9,R/W,REG_PID_TD,uint16,Td*1000
   10-11,R/W,REG_PID_ILIM,uint16,Ilim
   12-13,R/W,REG_POWER1,int16,"Left motor power, -1000...1000. Used both in PID on and PID off modes, see details in "
   14-15,R/W,REG_POWER2,int16,"Right motor power, -1000...1000"
   16,R/W,REG_REVERSE,byte,Bit0: should readings of encoder 0 be reversed? Bit2: same for encoder2
   24,R,REG_FW_VERSION,uint8,Firmware version - minor
   25,R,,uint8,Firmware version - major
   26,R,REG_WHO_AM_I,uint8,0x54 (used for testing connection)
   27,R,REG_STATUS,uint8,"Status. Bit0 is motor1, bit1 is motor2. If motor is disabled, corresponding bit is set; otherwise, bit=0"
   28-31,R,REG_ENCODER1,int32,"Encoder 1 value, ticks"
   32-35,R,REG_ENCODER2,int32,"Encoder 2 value, ticks"
   36-37,R,REG_SPEED1,int16,"Motor 1 speed, in ticks/s"
   38-39,R,REG_SPEED2,int16,"Motor 2 speed, in ticks/s"
