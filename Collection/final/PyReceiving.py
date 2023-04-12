import time
import serial
      




ser = serial.Serial(            
port='/dev/ttyACM0', #decide which port
baudrate = 9600,

'''
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
bytesize=serial.EIGHTBITS,
'''

 timeout=1
)

ser.reset_input_buffer()

while True:
    #n = random.randint(1,9)
    
    if(ser.in_waiting > 0):
     message = ser.readline().decode('utf-8').rstrip()
     #ser.read("Start up")
     print(message)
     
     #time.sleep(1)

     #feedback = ser.read()
     #print(feedback) #Expecting 'X'