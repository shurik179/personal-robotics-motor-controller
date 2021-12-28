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

.. function:: status()

.. function:: enable()

.. function:: disable()

.. function:: fw_version()

Basic motor control
-------------------

.. function:: set_motor(motor, power)

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
