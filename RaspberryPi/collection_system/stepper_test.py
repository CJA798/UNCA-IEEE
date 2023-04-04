#!/usr/bin/env python3
""" test example file for rpiMotorlib.py DRV8825 NEMA"""

import time 
import RPi.GPIO as GPIO

# Next 3 lines for development, local library testing import
# Comment out in production release and change RpiMotorLib.A4988Nema to A4988Nema
#import sys
#sys.path.insert(0, '/home/pi/Documents/tech/RpiMotorLib/RpiMotorLib')
#from RpiMotorLib import A4988Nema 

# Production installed library import 
from RpiMotorLib import RpiMotorLib

"""
# Comment in to Test motor stop put push button to VCC on GPIO 17 
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
"""

def main():
    enable = 14
    GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD  
    GPIO.setup(enable, GPIO.OUT)           # set GPIO24 as an output

    GPIO.output(enable, 1)         # set GPIO14 to 1/GPIO.HIGH/True
    print(GPIO.input(14))
    """main function loop"""
    # Comment in to Test motor stop , put push button to VCC on GPIO 17 
    # GPIO.add_event_detect(17, GPIO.RISING, callback=button_callback)
    
    # ====== Tests for motor ====
    #GPIO pins 
    # Microstep Resolution M0-M1-M2 -> GPIO Pin
    # Note: you can Pass in (-1,-1,-1) if you wish to hardwire Ms-X to logic and 
    # Save GPIO pins. 
    GPIO_pins = (-1, -1, -1)
    
    direction = 16       # Direction -> GPIO Pin WHITE 
    step = 26      # Step -> GPIO Pin BLUE

    # Declare an named instance of class pass GPIO-PINs
    #( self, direction_pin, step_pin, mode_pins ,motor_type)
    mymotortest = RpiMotorLib.A4988Nema(direction, step, GPIO_pins, "DRV8825")

    # ====================== section A ===================
    print("TEST SECTION A")
    #input("TEST: Press <Enter> to continue  full 180 clockwise Test2")
    mymotortest.motor_go(False, "Full" , 10000, .0000000000000000005, True, 0)

    # motor_go(clockwise, steptype, steps, stepdelay, verbose, initdelay)
    #input("TEST: Press <Enter> to continue  Full 180 turn Test1")
    mymotortest.motor_go(False, "Full" , 1000000, .00000000000005, True, 0)
    time.sleep(1)
    input("TEST: Press <Enter> to continue  full 180 clockwise Test2")
    mymotortest.motor_go(False, "Full" , 10000, .0000000005, True, .05)
    time.sleep(1)
    input("TEST: Press <Enter> to continue  full 180 no verbose Test3")
    mymotortest.motor_go(False, "Full" , 10000, .0000000005, False, .05)
    time.sleep(1) 
    input("TEST: Press <Enter> to continue  timedelay Test4")
    mymotortest.motor_go(True, "Full" , 10000, 0.0000001, True, .05)
    time.sleep(1)
    input("TEST: Press <Enter> to continue  full initdelay Test5")
    mymotortest.motor_go(True, "Full" , 90000, .000000001, True, 10)
    time.sleep(1)
    
    # ========================== section B =========================
    print("TEST SECTION B")
    
    # motor_go(clockwise, steptype, steps, stepdelay, verbose, initdelay)
    input("TEST: Press <Enter> to continue  half Test6")
    mymotortest.motor_go(False, "Half" , 400, .005, True, .05)
    time.sleep(1)
    input("TEST: Press <Enter> to continue 1/ 4 Test7")
    mymotortest.motor_go(False, "1/4" , 800, .005, True, .05)
    time.sleep(1)
    input("TEST: Press <Enter> to continue 1/8 Test8")
    mymotortest.motor_go(False, "1/8" , 1600, .005, True, .05)
    time.sleep(1)
    input("TEST: Press <Enter> to continue  1/16 Test9")
    mymotortest.motor_go(False, "1/16" , 3200, .005, True, .05) 
    time.sleep(1)
    input("TEST: Press <Enter> to continue  1/32 Test10")
    mymotortest.motor_go(False, "1/32" , 6400, .005, True, .05) 
    time.sleep(1)
    
"""
# needed for testing motor stop 
def button_callback(channel): 
    print("Test file: Stopping motor")
    mymotortest.motor_stop()   
"""
    
# ===================MAIN===============================

if __name__ == '__main__':
   
    print("TEST START")
    main()
    GPIO.cleanup() # Optional
    print("TEST END")
    exit()
    
    

# =====================END===============================
