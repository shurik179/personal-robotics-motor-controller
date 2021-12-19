import machine


MD_DEFAULT_I2C_ADDRESS = 0x54


MD_REG_ENABLE          = const(0)
MD_REG_PID_MODE        = const(1)
MD_REG_MAX_SPEED       = const(2)
MD_REG_PID_KP          = const(4)
MD_REG_PID_TI          = const(6)
MD_REG_PID_TD          = const(8)
MD_REG_PID_ILIM        = const(10)
MD_REG_POWER1          = const(12)
MD_REG_POWER2          = const(14)
MD_REG_ENC_RESET       = const(16)

# Read-only registers
MD_REG_FW_VERSION      = const(24)
MD_REG_WHO_AM_I        = const(26)
MD_REG_STATUS          = const(27)
MD_REG_ENCODER1        = const(28)
MD_REG_ENCODER2        = const(30)
MD_REG_SPEED1          = const(32)
MD_REG_SPEED2          = const(34)


class md():
    def __init__(self, i2c, address=MD_DEFAULT_I2C_ADDRESS):
        self._i2c = i2c
        self._addr = address
        #check connection
        who_am_i = self._read_8(MD_REG_WHO_AM_I)
        if who_am_i != MD_DEFAULT_I2C_ADDRESS:
            print(who_am_i);
            raise RuntimeError('Could not find motor driver, is it connected and powered? ')
        else:
            print("Motor driver  initialized")
            # enable motors
            self._i2c.writeto_mem(self._addr, MD_REG_ENABLE, bytes([1]))


######### BASIC  FUNCTIONS ################################
    def enable(self):
        self._write_8(MD_REG_ENABLE, 1)

    def disable(self):
        self._write_8(MD_REG_ENABLE, 0)

    def status(self):
        return self._read_8(MD_REG_STATUS)

######## MOTOR CONTROL, NO PID ##################################

    def set_motor(self, motor, power):
        if (motor == 0):
            self._write_16(MD_REG_POWER1, power)
        elif (motor == 1):
            self._write_16(MD_REG_POWER2, power)
        else:
            raise RuntimeError('Invalid motor number')

    def set_motors(self,  power1, power2=None):
        if power2 is None:
            power2 = power1
        self._write_16_array(MD_REG_POWER1, [power1, power2])

######## encoders  ##################################
#FIXME

##########  I2C UTILITY  ########################################
    def _write_8(self, register, data):
        # Write 1 byte of data to the specified  register address.
        self._i2c.writeto_mem(self._addr, register, bytes([data]) )

    def _write_16(self, register, data):
        # Write a 16-bit little endian value to the specified register
        # address.
        self._i2c.writeto_mem(self._addr, register, bytes([data & 0xFF,(data >> 8) & 0xFF]) )

    def _write_16_array(self, register, data):
        # write an array of littel endian 16-bit values  to specified register address
        l=len(data)
        buffer=bytearray(2*l)
        for i in range(l):
            buffer[2*i]=data[i] & 0xFF
            buffer[2*i+1]=(data[i]>>8) & 0xFF
        self._i2c.writeto_mem(self._addr, register, buffer)


    def _read_8(self, register):
        # Read and return a byte from  the specified register address.
        self._i2c.writeto(self._addr, bytes([register]))
        result = self._i2c.readfrom(self._addr, 1)
        return result[0]

    def _read_16(self, address):
        # Read and return a 16-bit signed little  endian value  from the
        # specified  register address.
        self._i2c.writeto(self._addr, bytes([register]))
        in_buffer = self._i2c.readfrom(self._addr, 2)
        raw =  (in_buffer[1] << 8) | in_buffer[0]
        if (raw & (1<<15)): # sign bit is set
            return (raw - (1<<16))
        else:
            return raw

    def _read_16_array(self, register, result_array):
        # Read and  saves into result_arrray a sequence of 16-bit little  endian
        # values  starting from the specified  register address.
        l=len(result_array)
        self._i2c.writeto(self._addr, bytes([register]))
        in_buffer = self._i2c.readfrom(self._addr, 2*l)
        for i in range(l):
            raw = (in_buffer[2*i+1] << 8) | in_buffer[2*i]
            if (raw & (1<<15)): # sign bit is set
                result_array[i] = (raw - (1<<16))
            else:
                result_array[i] = raw

    def _read_32(self, address):
        # Read and return a 32-bit signed little  endian value  from the
        # specified  register address.

        self._i2c.writeto(self._addr, bytes([register]))
        in_buffer = self._i2c.readfrom(self._addr, 4)
        raw = (in_buffer[3] << 24) |(in_buffer[2] << 16) | (in_buffer[1] << 8) | in_buffer[0]
        if (raw & (1<<31)): # sign bit is set
            return (raw - (1<<32))
        else:
            return raw
