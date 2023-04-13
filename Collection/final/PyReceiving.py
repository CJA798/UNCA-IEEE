import time
import serial
    


# define button pin
# check if pin is HIGH/Low
# if button is pressed :


ser = serial.Serial(port='/dev/ttyACM0',baudrate = 9600,timeout=1)

ser.reset_input_buffer()

while True:
    if(ser.in_waiting > 0):
     message = ser.readline().decode('utf-8').rstrip()
     print(message)