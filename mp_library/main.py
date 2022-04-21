from machine import Pin, I2C
from time import sleep
import PRMC
#For Pico
#i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000)
#FOR RP2040 ItsyBitsy
i2c = I2C(1, scl=Pin(3), sda=Pin(2), freq=100000)

# when using default i2c address:
driver = PRMC.controller(i2c)
# otherwise, use this form:
# driver = PRMC.controller(i2c, i2c_address)




print("Motor controller initialized.")
print("Firmware version: " + driver.fw_version())
# driver.reverse_encoder(0)
driver.set_motors(0.5) #set speed of both motors to 50%
while True:
    driver.get_encoders()
    driver.get_speeds()
    print("Encoders: {} {}. Speeds: {} {}".format(driver.encoder[0],driver.encoder[1], driver.speed[0], driver.speed[1] ))
    sleep(0.5)
