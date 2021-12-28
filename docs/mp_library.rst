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
    Note that motor can be disabled either because the user disabled it using ```disable```
    function above, or because one of the protection features (overcurrent,
    overtemperature, short circuit) was triggered.

    Overtemperature and other protection features are latching: if the
    protection was tirggered, the motor stays disabled even if the
    temperature/voltage is now normal. To re-enable the motors, use `enable` command above.


Basic motor control
-------------------

.. function:: set_motor(index, power)

   Sets the power for given motor (index=0 for MOTOR1, and index = 1 for MOTOR2).
   The power ranges between -1000 (full speed backwards) to 1000 (full speed forwards)

.. function:: set_motors(power1, power2 = None)




Encoders and speed
------------------

.. function:: get_encoder(index)

.. function:: get_speed(index)


.. function:: get_encoders()

.. function:: get_speeds(index)

.. function:: reverse_encoder(index)


PID configuration
-----------------

.. function:: configure_pid(maxspeed, Kp, Ti, Td, Ilim)

.. function:: configure_pid(maxspeed)

.. function:: pid_on()

.. function:: pid_off()
