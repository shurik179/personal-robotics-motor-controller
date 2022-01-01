# Personal Robotics Motor Controller


This repository contain design files for a 2-channel motor
controller board, designed by members of Personal Robotics discord server.


The controller is based on TLE9201 motor driver ICs by Infineon. The  key
features of this controller are listed below:

* Support for 2 brushed DC motors, 5-18v

* Current limit per channel: 5A continuous (with heatsinks), up to 8A peak.

* Short circuit, undervoltage, and overtemperature protection

* Support for quadrature encoders for each motor (3.3v)

* An on-board MCU (RP2040) for reading encoder input and
  providing variety of closed loop motor control modes

* I2C interface for control by a microcontroller or single board computer

This repository contains both board design files (as KiCad 6.0RC1 project) and
firmware (written using Arduino IDE with Earl Philhower arduino-pico core for RP2040).

Detailed project information is available http://motor-controller.rtfd.io/

## Copyright

This project was created in 2021  by:

* Ichiro Furusato  (https://github.com/ifurusato)

* Adam Green (https://github.com/adamgreen/)

* shurik179 (https://github.com/shurik179)

It is released under Apache license; see file LICENSE for details.
