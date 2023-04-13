import RPi.GPIO as GPIO
import time
import monitor_script
import os
import subprocess

#this code needs to be tested


# set up GPIO pins
GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.IN) # replace 17 with the GPIO pin number you are using
GPIO.setup(3, GPIO.OUT) # replace 18 with the GPIO pin number for the LED

GPIO.output(3, GPIO.LOW)

# wait for limit switch to be activated
while GPIO.input(2):
    time.sleep(0.1)
    GPIO.output(3, GPIO.LOW) # this probably should be high
   # subprocess.call(['sh', './test.sh']) #this should work
    subprocess.call(['/bin/bash', '/home/pi/UNCA-IEEE/Collection/final/monitor_script.sh']) #this should work
   #os.system("bash /home/pi/UNCA-IEEE/Collection/final/monitor_script.sh") this is not working



# turn on LED when limit switch is activated

#here you should call the code that you want to execute 
GPIO.output(3, GPIO.HIGH) #this ptobably should be Low

# print message when limit switch is activated
print("Limit switch activated")

# wait for a few seconds
time.sleep(5)

# turn off LED
GPIO.output(3, GPIO.LOW)

# clean up GPIO pins
GPIO.cleanup()