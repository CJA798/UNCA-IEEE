#!/usr/bin/env python3
""" test example file for rpiMotorlib.py DRV8825 NEMA"""

import time 
import RPi.GPIO as GPIO


def main():
    pin = 2
    GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD  
    GPIO.setup(pin, GPIO.OUT)           # set GPIO24 as an output

    GPIO.output(pin, 1)         # set GPIO14 to 1/GPIO.HIGH/True
    print(GPIO.input(pin))
    GPIO.cleanup() # Optional


if __name__ == '__main__':
   
    print("TEST START")
    main()
    #GPIO.cleanup() # Optional
    print("TEST END")
    exit()
    
    