from machine import Pin, UART 
from time import sleep
import PRMC

uart0 = UART(0, baudrate=76800, tx=Pin(0), rx=Pin(1), timeout = 3, timeout_char = 2)

driver = PRMC.controller(uart0, ID = 1)




print("Motor controller initialized.")
print("Firmware version: " + driver.fw_version())
# driver.reverse_encoder(0)
driver.set_motors(0.5) #set speed of both motors to 50%
while True:
    driver.get_encoders()
    #sleep(0.001)
    driver.get_speeds()
    print("Encoders: {} {}. Speeds: {} {}".format(driver.encoder[0],driver.encoder[1], driver.speed[0], driver.speed[1] ))
    driver.get_currents()
    print("Current: {} {}".format(driver.current[0],driver.current[1] ))

    sleep(0.5)
