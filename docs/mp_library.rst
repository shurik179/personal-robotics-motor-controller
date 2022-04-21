.. _library:
**************************
Micropython Library
**************************
This chapter documents included Micropython library.

Installation
============
To install the library, download the latest version of `PRMC.py` file
from `mp_library` folder in |github| and copy it to the root directory of your board.



Basic example
=============

Below is the basic example of Micropython code using the library.

.. code-block:: python

    from machine import Pin, I2C
    from time import sleep
    import PRMC
    i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000)

    driver = PRMC.controller(i2c)

    print("Motor controller initialized.")
    print("Firmware version: " + driver.fw_version()))
    # driver.reverse_encoder(0)
    driver.set_motors(0.5) #set speed of both motors to 50%
    while True:
        driver.get_encoders()
        driver.get_speeds()
        print("Encoders: {} {}. Speeds: {} {}".format(driver.encoder[0],driver.encoder[1], driver.speed[0], driver.speed[1] ))
        sleep(0.5)

Full documentation
==================

In all the functions which use `motor` as an argument, value of 0 corresponds to
Motor1, and value of 1, to Motor2.

Initialization and basic info
-----------------------------

.. function:: controller(i2c, address=0x54)

    Creates and initializes motor controller object. If connection can't be established
    (e.g. because the driver is not connected or malfunctions), an exception will be raised.
    Optional parameter `address` is the I2C address. If omitted, default value of
    `PRMC_DEFAULT_I2C_ADDRESS=0x54` is used.


.. function:: fw_version()

   Returns firmware version as a string, in format `major.minor`, e.g. `1.99`


.. function:: disable(motor_combo)

   Disable motor outputs. Possible values for `motor_combo` are:

   * `1`: disable motor 1, enable motor 2

   * `2`: enable motor 1, disable motor 2

   * `3`: disable both motors

.. function:: enable(motor_combo)

   Opposite of the previous command, setting which motors should be enabled.

   Possible values for `motor_combo` are:

  * `1`: enable motor 1, disable motor 2

  * `2`: disable motor 1, enable motor 2

  * `3`: enable both motors

  Note: according to the datasheet, if a motor driver was disabled by a triggered
  protection feature (e.g., overtemperature protection), to enable it again you need
  to first send disable signal and then enable signal:

  .. code-block:: python

      driver.disable(1) # to disable motor 1, leaving motor 2 enabled
      driver.enable(3)  # to reenable both motors




.. function:: motor_status(motor)

    Returns the motor status.
    If motor is enabled, status is 0; if it is disabled, status is 1.
    Note that motor can be disabled either because the user disabled it using `disable`
    function above, or because one of the protection features (overcurrent,
    overtemperature, short circuit) was triggered.

    Overtemperature and other protection features are latching: if the
    protection was triggered, the motor stays disabled even after the
    temperature/voltage returns to normal. To re-enable the motors, use `enable` command above.


Basic motor control
-------------------

.. function:: set_motor(motor, power)

   Sets the power for given motor.
   The power ranges between -1.0 (full speed backwards) to 1.0 (full speed forwards)

.. function:: set_motors(power1, power2 = None)

   Set power for both motors in one command. Argument `power2` is optional;
   if omitted, same power is given to both motors.




Encoders and speed
------------------

.. function:: get_encoder(motor)

   Returns current encoder reading (ticks) for given motor. If encoder is absent or
   disconnected, returns 0.

   When counting ticks, both rising and falling edge is counted, for each of 2 channels.
   Thus, we get 4 ticks per period.

.. function:: get_speed(motor)

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

.. function:: reverse_encoder(motor)

   After calling this function, all future readings of this encoder will be
   reversed (multiplied by -1). This is convenient if your encoder and motor are wired so that
   positive power to the motors caused speed measured by encoder to be negative.



PID configuration
-----------------
To use PID mode (as described in `Firmware` section), you need to set PID
coefficients. You can do it using functions below.


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
