.. _library:
**************************
Micropython Library
**************************
This chapter documents included Micropython library.

Installation
============
To install the library, download the latest version of `motor_driver.py` file
from `mp_library` folder in |github| and copy it to the root directory of your board.



Basic example
=============

Below is the basic example of Micropython code using the library.

.. code-block:: python

   from machine import Pin, I2C
   from time import sleep
   import motor_driver
   i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000)
   driver = motor_driver.md(i2c)

   driver.set_motors(500,500) #motor speed range: -1000...1000
   while True:
      print(driver.get_encoder(0))
      print(driver.get_speed(0))
      sleep(0.5)

Full documentation
==================

Initialization and basic info
-----------------------------

.. function:: md(i2c, address=MD_DEFAULT_I2C_ADDRESS)

    Creates and initializes motor driver object. If connection can't be established
    (e.g. because the driver is not connected or malfunctions), an exception will be raised.
    Optional parameter `address` is the I2C address. If omitted, default value of
    `MD_DEFAULT_I2C_ADDRESS=0x54` is used.


.. function:: fw_version()

   Returns firmware version as a string, in format `major.minor`, e.g. `1.99`

.. function:: disable()

   Disables both motors.


.. function:: enable()

   Re-enables both motors. (Motors are initially enabled.)


.. function:: motor_status(index)

    Returns status of motor with given index (index=0 for MOTOR1, index=1 for MOTOR2).
    If motor is enabled, status is 0; if it is disabled, status is 1.
    Note that motor can be disabled either because the user disabled it using `disable`
    function above, or because one of the protection features (overcurrent,
    overtemperature, short circuit) was triggered.

    Overtemperature and other protection features are latching: if the
    protection was triggered, the motor stays disabled even after the
    temperature/voltage returns to normal. To re-enable the motors, use `enable` command above.


Basic motor control
-------------------

.. function:: set_motor(index, power)

   Sets the power for given motor (index=0 for MOTOR1, and index = 1 for MOTOR2).
   The power ranges between -1000 (full speed backwards) to 1000 (full speed forwards)

.. function:: set_motors(power1, power2 = None)

   Set power for both motors in one command. Argument `power2` is optional;
   if omitted, same power is given to both motors.




Encoders and speed
------------------

.. function:: get_encoder(index)

   Returns current encoder reading (ticks) for given motor. If encoder is absent or
   disconnected, returns 0.

   When counting ticks, both rising and falling edge is counted, for each of 2 channels.
   Thus, we get 4 ticks per period.

.. function:: get_speed(index)

   Returns current speed reading for given motor, in ticks/sec.



.. function:: get_encoders()

   Gets from the controller and saves readings of both encoders. These readings
   can be accessed using properties `controller.encoder[0]` and `controller.encoder[1]`.

   Using this method instead of `get_encoder(0)` followed by `get_encoder(1)` ensures
   that both encoder readings were taken at same moment of time, which might be
   important when comparing them.

.. function:: get_speeds()

    Gets from the controller and saves readings of both speeds. These readings
    can be accessed using properties `controller.speed[0]` and `controller.speed[1]`.
    As before, advantage of this method is that both speeds are read at the same moment of time.

.. function:: reverse_encoder(index)

   After calling this function, all future readings of this encoder will be
   reversed (multiplied by -1). This is convenient if your encoder and motor are wired so that
   positive power to the motors caused speed measured by encoder to be negative.



PID configuration
-----------------

Motor driver firmware provides an option of closed loop motor control. In this mode,
the controller tries to keep the speed of each motor (as measured using encoders)
as close as possible to the desired speed, using industry-standard PID
(proportional-integral-derivative) algorithm.

The motor power is determined by the standard formula of PID algorithm:

.. math::
   P=P_0+ K_p e+\frac{K_p}{T_i}\int e\, dt +K_p T_d \frac{d}{dt} e

where:

:math:`P` is motor power (ranging from -1.0 to 1.0)

:math:`P_0=v_{desired}/v_{max}` is the zero-level approximation; here
:math:`v_{desired}` is the requested speed (in ticks/s) and :math:`v_{max}` is the maximal
possible motor speed, which is determined by motor's no-load RPM.

:math:`e=v_{desired}-v_{actual}` is the error, i.e. the  difference of desired and actual motor
speeds (measured in encoder ticks/sec)

:math:`\int e dt` and :math:`\frac{d}{dt}e` are the integral and derivative
of the error, measured in encoder tics and tics/:math:`sec^2` respectively

:math:`K_p, T_i, T_d` are the PID coefficients.


The behavior of the PID algorithm is determined by these coefficients; for
example, if the coefficient :math:`K_p` is too small, it will take the motor a
long time to stabilize to the desired speed; if the coefficient :math:`K_p` is
too large, you might get oscillations. Choosing the correct coefficients
requires significant experience and is certainly outside the scope of this user
guide.



.. function:: configure_pid(maxspeed, Kp, Ti, Td, Ilim)

   Sets the PID coefficients for both motors. Note that these coefficients are
   only used if you enable PID using `pid_on()` command below.

.. function:: configure_pid(maxspeed)

   Sets default PID coefficients, based on motor maximal speed (in ticks/s).
   The default values are as follows:

.. code-block:: python

    Kp = 0.8/maxspeed
    Ti = 0.3
    Td = 0.03
    Ilim = 1000

These values are somewhat conservative: while they are unlikely to cause
oscillations, it may take some time for the motor speed to stabilize. You may
experiment with these values to improve performance of the PID loop.




.. function:: pid_on()

   Enables PID for both motors. This assumes that PID has been configured
   previously using `configure_pid()` command.

   After enabling PID, any power given to the motors using  `set_motor` commands
   will be actively maintained using PID algorithm.




.. function:: pid_off()

   Disables PID for both motors.


It is suggested that you only use the commands above when the motors are stopped.
Changing PID settings while the motors are in motion can lead to unexpected results.