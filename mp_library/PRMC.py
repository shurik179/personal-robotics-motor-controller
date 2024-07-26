import machine
from time import sleep
from ucollections import namedtuple

PRMC_MOTOR_MAX_SPEED = 1000

PRMC_REG_ENABLE          = const(0)
PRMC_REG_REVERSE         = const(1)
PRMC_REG_PID_MODE        = const(2)
PRMC_REG_MAX_SPEED       = const(4)
PRMC_REG_PID_KP          = const(6)
PRMC_REG_PID_TI          = const(8)
PRMC_REG_PID_TD          = const(10)
PRMC_REG_PID_ILIM        = const(12)
PRMC_REG_POWER1          = const(14)
PRMC_REG_POWER2          = const(16)

# Read-only registers
PRMC_REG_MOTOR_ID        = const(20)
PRMC_REG_FW_VERSION      = const(21)
PRMC_REG_STATUS          = const(23)
PRMC_REG_CURRENT1        = const(24)
PRMC_REG_CURRENT2        = const(26)
PRMC_REG_ENCODER1        = const(28)
PRMC_REG_ENCODER2        = const(32)
PRMC_REG_SPEED1          = const(36)
PRMC_REG_SPEED2          = const(38)

# Motor enable masks
PRMC_ENABLE_M1 = const (1)
PRMC_ENABLE_M2 = const (2)
PRMC_ENABLE_BOTH = const (3)

# DMX error codes 
PRMC_NO_ERROR = const(0)
PRMC_RECEIVE_ERROR = const(0x80)  #this is added - not part of dynamixel protocol
PRMC_CHECKSUM_ERROR	= const(0x10)

# DMX instruction codes
PRMC_INSTR_READ = const(0x02)
PRMC_INSTR_WRITE = const (0x03)




class controller():
    def __init__(self, uart, ID):
        self._uart = uart
        self._ID = ID #integer 
        self._error = PRMC_NO_ERROR
        self.encoder=[0,0]
        self.speed=[0,0]
        self.current=[0,0] #motor current in mA

        #check connection
        readID = self._read_reg(PRMC_REG_MOTOR_ID, 1)
        if readID != self._ID:
            print("Failed to connect to the controller")
            self._error = PRMC_RECEIVE_ERROR
        else:
            # enable motors
            self.set_motors(0)
            self.pid_off()
            self.disable(PRMC_ENABLE_BOTH)
            sleep(0.1)
            self.enable(PRMC_ENABLE_BOTH)
            print("Motor driver  initialized")


######### BASIC  FUNCTIONS ################################
    def disable(self,mask):
        self._write_reg(PRMC_REG_ENABLE, bytes([PRMC_ENABLE_BOTH-mask]) ) # effectively, bitwise negation of 2-bit integer

    def enable(self,mask):
        self._write_reg(PRMC_REG_ENABLE, bytes([mask]) )

    def status(self,index):
        raw_status=self._read_reg(PRMC_REG_STATUS, 1)
        if index == 0:
            return (raw_status & 1)
        if index == 1:
            return (raw_status & (1<<1))

    def fw_version(self):
        minor = self._read_reg(PRMC_REG_FW_VERSION, 1)
        major = self._read_reg(PRMC_REG_FW_VERSION + 1, 1)
        return("{}.{}".format(major,minor))


######## MOTOR CONTROL, NO PID ##################################

    def set_motor(self, motor, power):
        value = round(power*PRMC_MOTOR_MAX_SPEED)
        params = value.to_bytes(2, 'little') #convert to 2-byte array in little endian encoding 
        if (motor == 0):
            result = self._write_reg(PRMC_REG_POWER1, params)
        elif (motor == 1):
            result = self._write_reg(PRMC_REG_POWER2, params)
        else:
            raise RuntimeError('Invalid motor number')
        return(result)

    def set_motors(self,  power1, power2=None):
        if power2 is None:
            power2 = power1
        #convert powers to bytearray (2 bytes per integer)
        params = self._list_to_bytearray([power1 * PRMC_MOTOR_MAX_SPEED, power2 * PRMC_MOTOR_MAX_SPEED], 2)
        result = self._write_reg(PRMC_REG_POWER1, params)
        return(result)
    
######## MOTOR CONTROL WITH PID ##################################

    def configure_pid(self, maxspeed, Kp = None, Ti = None, Td = None, Ilim = None):
        if Kp is None:
            Kp = 0.8/maxspeed
            Ti = 0.3
            Td = 0.03
            Ilim = 1000
        data = [maxspeed, Kp*10000000, Ti*1000, Td*1000, Ilim]
        self._write_reg(PRMC_REG_MAX_SPEED, self._list_to_bytearray(data,2))

    def pid_on(self):
        self._write_reg(PRMC_REG_PID_MODE, bytes([1]) )

    def pid_off(self):
        self._write_reg(PRMC_REG_PID_MODE, bytes([0]))


######## encoders, currents ##################################
    def reverse_encoder(self, i):
        if i==0:
            self._write_reg(PRMC_REG_REVERSE, bytes([1]) )
        else:
            self._write_reg(PRMC_REG_REVERSE, bytes([4]) )

    def get_encoders(self):
        result = self._read_list(self.encoder,PRMC_REG_ENCODER1, 4) #4-byte values 
        return(result)

    def get_speeds(self):
        result = self._read_list(self.speed,PRMC_REG_SPEED1, 2) # 2-bytes values
        return(result)
    
    def get_encoder(self,i):
        if i==0:
            return(self._read_reg(PRMC_REG_ENCODER1, 4))
        else:
            return(self._read_reg(PRMC_REG_ENCODER2, 4))

    def get_speed(self,i):
        if i==0:
            return(self._read_reg(PRMC_REG_SPEED1, 2))
        else:
            return(self._read_reg(PRMC_REG_SPEED2, 2))

    def get_currents(self):
        result = self._read_list(self.current,PRMC_REG_CURRENT1, 2) # 2-bytes values
        return(result)
    
##########  UART UTILITY  ########################################
    def _send_packet(self, instr, params):
        # creates and sends an instruction packet, see https://emanual.robotis.com/docs/en/dxl/protocol1/ 
        length = len(params)+2
        sum = self._ID + length + instr
        for p in params:
            sum += p
        sum = sum & 0xFF #make sure it is in range 0..255

        checksum = 0xFF - sum 
        buf = bytes([0xFF, 0xFF, self._ID, length, instr])+params+bytes([checksum])
        self._uart.write(buf)

    def _receive_packet(self):
        # receives a status packet and verifies ID and checksum 
        # returns a tuple (error, params)
        error = PRMC_RECEIVE_ERROR
        params = []
        buf = self._uart.read(4) # read 4 bytes 
        if (buf is None or len(buf)<4) : #timeout - didn't get enough data
            #print("no response")
            return ( (error, params)) 
        if  not (buf[0] == 0xFF and buf[1] == 0xFF and buf[2] == self._ID) :
            #print("invalid response")
            return ( (error, params)) 
            # response doesn't start with 0xFF 0xFF followed by correct ID
        #if we are here, we received first 4 bytes correctly
        length = buf[3]
        #let's read the remaining bytes
        buf2 =  self._uart.read(length) 
        if (buf2 is None or len(buf2) < length): # timeout
            #print(length)
            #print(len(buf2))
            #print(buf2)
            return( (error, params) )
        error = buf2[0]
        params = buf2[1:-1]
        checksum = buf2[-1]
        sum = self._ID + length
        for x in buf2[:-1]:
            sum += x # add all entries of buf 2 except the last one, checksum
        sum = sum & 0xFF
        if (checksum + sum != 0xFF):
            error = PRMC_CHECKSUM_ERROR
            #print("checksum error")
        else:
            #print("success")
            pass
        return((error, params))


    def _write_reg(self, register, params):
        #first, flush the RX buffer
        if self._uart.any() > 0: 
            self._uart.read() 
        self._send_packet(PRMC_INSTR_WRITE, bytes([register])+params)
        (error, params)= self._receive_packet()
        return(error)

    # read a single int value from (possibly multibyte) register
    # return the read value or None on failure 
    def _read_reg(self, register, num_bytes):
        #first, flush the RX buffer
        if self._uart.any() > 0: 
            self._uart.read() 
        self._send_packet(PRMC_INSTR_READ, bytes([register, num_bytes]) )
        (error, params) = self._receive_packet()
        if error == PRMC_NO_ERROR:
            return(int.from_bytes(params, 'little')) # use little endian encoding
        else:
            return(None)

    # read an array of  int values from (possibly multibyte) register, saving to given list 
    # num_bytes is number of bytes per integer (e.g. for int16, we would have num_bytes = 2)
    # returns error code 
    def _read_list(self, output_list, register, num_bytes):
        #first, flush the RX buffer
        if self._uart.any() > 0: 
            self._uart.read()
        # prepare and send instruction packet 
        N = len(output_list)
        self._send_packet(PRMC_INSTR_READ, bytes([register, N * num_bytes]))
        (error, params) = self._receive_packet()
        if error == PRMC_NO_ERROR:
            self._bytearray_to_list(params, output_list, num_bytes)
        return(error)

                
    # convert list of numbers to a bytearray
    # rounds each number to int and converts it to num_bytes byte sequence in little endian encoding 
              
    def _list_to_bytearray(self, l, num_bytes = 1):
        N = len(l)
        buf = bytearray ( N * num_bytes)
        for i in range(N):            
            buf[i*num_bytes:(i+1)*num_bytes] = round(l[i]).to_bytes(num_bytes, 'little')
        return(buf)
    
    # converts array of bytes to integers, saving them in given output list 
    def _bytearray_to_list(self, input_b,output_list, num_bytes = 1):
        N = len(input_b)/num_bytes
        
        for i in range(N):
            output_list[i] = int.from_bytes(input_b[i*num_bytes:(i+1)*num_bytes], 'little')
