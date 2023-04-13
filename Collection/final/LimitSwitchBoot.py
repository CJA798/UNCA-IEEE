import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.IN)

while not GPIO.input(2):
  time.sleep(0.1)


print("Limit Switch on")


