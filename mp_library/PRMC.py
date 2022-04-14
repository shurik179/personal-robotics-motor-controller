import machine


PRMC_DEFAULT_I2C_ADDRESS = 0x54

PRMC_MOTOR_MAX_SPEED = 1000



PRMC_REG_ENABLE          = const(0)
PRMC_REG_PID_MODE        = const(1)
PRMC_REG_MAX_SPEED       = const(2)
PRMC_REG_PID_KP          = const(4)
PRMC_REG_PID_TI          = const(6)
PRMC_REG_PID_TD          = const(8)
PRMC_REG_PID_ILIM        = const(10)
PRMC_REG_POWER1          = const(12)
PRMC_REG_POWER2          = const(14)
PRMC_REG_REVERSE         = const(16)
#PRMC_REG_ENC_RESET       = const(16)

# Read-only registers
PRMC_REG_FW_VERSION      = const(24)
PRMC_REG_WHO_AM_I        = const(26)
PRMC_REG_STATUS          = const(27)
PRMC_REG_ENCODER1        = const(28)
PRMC_REG_ENCODER2        = const(32)
PRMC_REG_SPEED1          = const(36)
PRMC_REG_SPEED2          = const(38)

# Motor enable masks
PRMC_ENABLE_M1 = const (1)
PRMC_ENABLE_M2 = const (2)
PRMC_ENABLE_BOTH = const (3)

class controller():
    def __init__(self, i2c, address=PRMC_DEFAULT_I2C_ADDRESS):
        self._i2c = i2c
        self._addr = address
        self.encoder=[0,0]
        self.speed=[0,0]
        #check connection
        who_am_i = self._read_8(PRMC_REG_WHO_AM_I)
        if who_am_i != PRMC_DEFAULT_I2C_ADDRESS:
            print(who_am_i);
            raise RuntimeError('Could not find motor driver, is it connected and powered? ')
        else:
            print("Motor driver  initialized")
            # enable motors
            self._i2c.writeto_mem(self._addr, PRMC_REG_ENABLE, bytes([PRMC_ENABLE_BOTH]))


######### BASIC  FUNCTIONS ################################
    def enable(self,mask):
        self._write_8(PRMC_REG_ENABLE, mask)

    def status(self,index):
        raw_status=self._read_8(PRMC_REG_STATUS)
        if index == 0:
            return (raw_status & 1)
        if index == 1:
            return (raw_status & (1<<1))

    def fw_version(self):
        minor = self._read_8(PRMC_REG_FW_VERSION)
        major = self._read_8(PRMC_REG_FW_VERSION + 1)
        return("{}.{}".format(major,minor))


######## MOTOR CONTROL, NO PID ##################################

    def set_motor(self, motor, power):
        if (motor == 0):
            self._write_16(PRMC_REG_POWER1, round(power))
        elif (motor == 1):
            self._write_16(PRMC_REG_POWER2, round(power))
        else:
            raise RuntimeError('Invalid motor number')

    def set_motors(self,  power1, power2=None):
        if power2 is None:
            power2 = power1
        self._write_16_array(PRMC_REG_POWER1, [round(power1), round(power2)])
######## MOTOR CONTROL WITH PID ##################################

    def configure_pid(self, maxspeed, Kp = None, Ti = None, Td = None, Ilim = None):
        if Kp is None:
            Kp = 0.8/maxspeed
            Ti = 0.3
            Td = 0.03
            Ilim = 1000
        data = [round(maxspeed), round(Kp*10000000), round(Ti*1000), round (Td*1000), round(Ilim)]
        self._write_16_array(PRMC_REG_MAX_SPEED, data)

    def pid_on(self):
        self._write_8(PRMC_REG_PID_MODE, 1)

    def pid_off(self):
        self._write_8(PRMC_REG_PID_MODE, 0)


######## encoders  ##################################
    def reverse_encoder(self, i):
        if i==0:
            self._write_8(PRMC_REG_REVERSE, 1)
        else:
            self._write_8(PRMC_REG_REVERSE, 4)

    def get_encoders(self):
        self._read_32_array(PRMC_REG_ENCODER1, self.encoder)

    def get_speeds(self):
        self._read_16_array(PRMC_REG_SPEED1, self.speed)

    def get_encoder(self,i):
        if i==0:
            return(self._read_32(PRMC_REG_ENCODER1))
        else:
            return(self._read_32(PRMC_REG_ENCODER2))

    def get_speed(self,i):
        if i==0:
            return(self._read_16(PRMC_REG_SPEED1))
        else:
            return(self._read_16(PRMC_REG_SPEED2))

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

    def _read_16(self, register):
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

    def _read_32(self, register):
        # Read and return a 32-bit signed little  endian value  from the
        # specified  register address.

        self._i2c.writeto(self._addr, bytes([register]))
        in_buffer = self._i2c.readfrom(self._addr, 4)
        raw = (in_buffer[3] << 24) |(in_buffer[2] << 16) | (in_buffer[1] << 8) | in_buffer[0]
        if (raw & (1<<31)): # sign bit is set
            return (raw - (1<<32))
        else:
            return raw


    def _read_32_array(self, register, result_array):
        # Read and  saves into result_arrray a sequence of 32-bit little  endian
        # values  starting from the specified  register address.
        l=len(result_array)
        self._i2c.writeto(self._addr, bytes([register]))
        in_buffer = self._i2c.readfrom(self._addr, 4*l)
        for i in range(l):
            raw = (in_buffer[4*i+3] << 24) |(in_buffer[4*i+2] << 16) | (in_buffer[4*i+1] << 8) | in_buffer[4*i]
            if (raw & (1<<31)): # sign bit is set
                result_array[i] = (raw - (1<<32))
            else:
                result_array[i] = raw
