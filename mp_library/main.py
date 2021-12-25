from machine import Pin, I2C
from time import sleep
import motor_driver
i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=100000) 

# just for testing 
result = i2c.scan()
print(result)

sleep(1.0)


driver = motor_driver.md(i2c)

print(driver.status())
driver.reverse_encoder(0)
driver.set_motors(600,300)
while True:
    print(driver.get_encoder(0))
    sleep(0.05)
    print(driver.get_speed(0))
    sleep(0.5)
