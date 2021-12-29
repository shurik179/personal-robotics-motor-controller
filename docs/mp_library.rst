.. _library:
**************************
Micropython Library
**************************
This chapter documents included Micropython library.

Installation
============
To install the library, download the latest version of `motor_driver.py` file
from |github| and copy it to the root directory of your board.



Basic example
=============

Below is the basic example of Micropython code using the library.

.. code-block:: python

   from machine import Pin, I2C
   from time import sleep
   import motor_driver
   i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000)
   driver = motor_driver.md(i2c)

   print(driver.status())
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

.. function:: configure_pid(maxspeed, Kp, Ti, Td, Ilim)

.. function:: configure_pid(maxspeed)

.. function:: pid_on()

   Enables PID for both motors. This assumes that PID has been configured previously.

   After enabling PID, any power given to the motors using  `set_motor` commands
   will be actively maintained using PID algorithm. 


.. function:: pid_off()

   Disables PID for both motors.
