.. _firmware:
**************************
Firmware and register map
**************************

Firmware
========

The firmware for the PRMC motor controller  is written in Arduino IDE using
Earl Philhower's `RP2040 core <https://github.com/earlephilhower/arduino-pico>`__.

If you want to modify it, you can find source files in `firmware` folder in |github|.


I2C communication
=================

PRMC is controlled via I2C bus. It supports speeds up to 400kHz.
Default I2C address is `0x54`, but it can be changed by closing the solder
bridge jumpers on the bottom of the board (see `specs`).

It uses the common register model: I2C master reads and writes data to registers.
The following table lists all PRMC I2C registers.

All multibyte registers use little-endian encoding; e.g. for register
`REG_POWER1`, low byte is at address 12, and high byte, at address 13.



.. csv-table:: Register map
   :widths: 10 10 30 10 50
   :header:   "register(s)","R/W","Name","data type","description"

   0,R/W,REG_ENABLE,byte,"Enable/disable motors. Bit0 controls motor1 (1=enable, 0=disable); bit1 controls motor2"
   1,R/W,REG_PID_MODE,byte,"0: normal control, PID off; 1: PID on (individual motors)"
   2-3,R/W,REG_MAX_SPEED,uint16,Max motor speed in encoder ticks/s. Required for PID
   4-5,R/W,REG_PID_KP,uint16,Kp*10^7
   6-7,R/W,REG_PID_TI,uint16,Ti*1000
   8-9,R/W,REG_PID_TD,uint16,Td*1000
   10-11,R/W,REG_PID_ILIM,uint16,Ilim
   12-13,R/W,REG_POWER1,int16,"Motor1 power, -1000...1000. Used both in PID on and PID off modes, see details in "
   14-15,R/W,REG_POWER2,int16,"Motor2 power, -1000...1000"
   16,R/W,REG_REVERSE,byte,Bit0: should readings of encoder 0 be reversed? Bit2: same for encoder2
   24,R,REG_FW_VERSION,uint8,Firmware version - minor
   25,R,,uint8,Firmware version - major
   26,R,REG_WHO_AM_I,uint8,0x54 (used for testing connection)
   27,R,REG_STATUS,uint8,"Status. Bit0 is motor1, bit1 is motor2. If motor is disabled, corresponding bit is set; otherwise, bit=0"
   28-31,R,REG_ENCODER1,int32,"Encoder 1 value, ticks"
   32-35,R,REG_ENCODER2,int32,"Encoder 2 value, ticks"
   36-37,R,REG_SPEED1,int16,"Motor 1 speed, in ticks/s"
   38-39,R,REG_SPEED2,int16,"Motor 2 speed, in ticks/s"


Motor control: basic
====================

By default, PRMC has both motor drivers enabled, with PID mode off. To set the
motor power just write a value between -1000 and 1000 to register `REG_POWER1`
for motor 1 (respectively, `REG_POWER2` for motor 2).

To disable one or both motor drivers, write the corresponding value to `REG_ENABLE`.
Note that the motor driver can also be disabled because one of the driver IC
protection features, such as overtemperature protection, were triggered.
You can check if a motor is enabled or disabled by reading `REG_STATUS`.
If necessary, you can re-enable a motor by writing to `REG_ENABLE`.

Encoders
========
PRMC supports quadrature encoders for each motor, recording encoder values
and speeds to registers `REG_ENCODERx` and `REG_SPEEDx`. It counts both
rising and falling edge for each of quadrature encoder channels, thus producing
4 ticks per encoder cycle. Both encoder values and speeds can be positive or
negative depending on the rotation direction.

Note that encoder value is recorded as 32-bit integer, so it will overflow at
2,147,483,648 ticks. Similarly, encoder speed is 16-bit integer, so it will
overflow at 32,768 ticks/s. This should be more than enough for common hobby
motors; e.g. a popular
`GoBilda Yellow Jacket <https://www.gobilda.com/5202-series-yellow-jacket-planetary-gear-motor-26-9-1-ratio-223-rpm-3-3-5v-encoder/>`__
motor has  no-load  speed of about 3000 encoder ticks/s.

For normal operation of PID control (see next section), it is expected that
providing positive power to the motor gives positive speed as measured by the encoder.
If your motor and encoder are wired so that positive power to motor gives negative
encoder counts, you can either change the wiring, or change the encoder direction
in software by writing to `REG_REVERSE`.

PID control
===========


Motor driver firmware provides an option of closed loop motor control. In this mode,
the controller tries to keep the speed of each motor (as measured using encoders)
as close as possible to the desired speed, using industry-standard PID
(proportional-integral-derivative) algorithm.

The motor power is determined by the standard formula of PID algorithm:

.. math::
   M=M_0+ P + I_{clipped} + D


where:

:math:`M` is motor power (ranging from -1.0 to 1.0)

:math:`M_0=v_{target}/v_{max}` is the zero-level approximation; here
:math:`v_{target}` is the requested speed (in ticks/s) and :math:`v_{max}` is the maximal
possible motor speed, which is determined by motor's no-load RPM

:math:`P=K_p e` is the proportional term. Here
:math:`e=v_{target}-v_{actual}` is the error, i.e. the  difference of desired and actual motor
speeds (measured in encoder ticks/sec) and :math:`K_p` is a coefficient

:math:`I=\frac{K_p}{T_i}\int e dt` is the integral term; :math:`I_{clipped}` is
:math:`I` but clipped to be between :math:`-I_{lim}` and :math:`I_{lim}`


:math:`K_p T_d \frac{d}{dt} e\frac{d}{dt}e` is the  derivative
of the error

:math:`K_p, T_i, T_d, I_{lim}` are the PID coefficients. They **must  be set
(by writing to corresponding registers) before PID mode can be used**; there
are no default values.


The behavior of the PID algorithm is determined by these coefficients; for
example, if the coefficient :math:`K_p` is too small, it will take the motor a
long time to stabilize to the desired speed; if the coefficient :math:`K_p` is
too large, you might get oscillations. Choosing the correct coefficients
requires significant experience and is certainly outside the scope of this user
guide.
