# Motor controller

**Warning: this project is currently work in progress and is not ready for public use.**

This repository contain design files for a 2-channel motor
controller board, designed by members of Personal Robotics discord server.


The controller is based on TLE9201 motor driver ICs by Infineon. The  key
features of this controller are listed below:

* Support for 2 brushed DC motors, 5v-18v

* Current limit per channel: 4A continuous, up to 8A peak.

* Overvoltage, undervoltage, and thermal overload protection

* Support for quadrature encoders for each motor (3.3v)

* An on-board motor controller IC (RP2040) for reading encoder input and providing variety of closed loop motor control modes

* I2C interface for control by a microcontroller or single board computer

This repository contains both board design files (as KiCad 6.0RC1 project) and firmware (written using Arduino IDE with Earl Philhower arduino-pico core for RP2040).
