import serial
#initialize serial connection
def init_serial():
      COMNUM = 5  # Set you COM port # here (Check it in device manager)
      global ser  # Must be declared in each fxn used
      ser = serial.Serial()
      ser.baudrate = 4800
      ser.port = COMNUM -1 # Starts at 0, so subtract 1
      ser.timeout = 1 # You must specify a timeout (in seconds) so that the serial port doesn't hang
      ser.open() # Open the serial port
      # print port open or closed
      if ser.isOpen():
          print ('Open: ')
init_serial()
while 1:
    # read what is on serial port
    data = ser.readline()   # reads in bytes followed by a newline
    print (data)