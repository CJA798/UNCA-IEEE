from time import sleep
from pi_servo_hat import PiServoHat
import RPi.GPIO as GPIO

# Production installed library import
from RpiMotorLib import RpiMotorLib

enable = 14

GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD  
GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(enable, GPIO.OUT)

GPIO_pins = (-1, -1, -1)  
direction = 16       # Direction -> White
step = 26      # Step -> Blue
DrumMotor = RpiMotorLib.A4988Nema(direction, step, GPIO_pins, "DRV8825")

GPIO.output(enable, 1)

try:
    while True:
        if GPIO.input(6):
            DrumMotor.stop_motor()
            break
        else:
            DrumMotor.motor_go(False, "Full", 1, .000000001, False, .0001)
            print(GPIO.input(6))
finally:
    GPIO.output(enable, 0)

'''
Ducks height = -110
Columns height = 0
Down = 230
'''

'''for i in range(0, 181, 1):
  servo_hat.move_servo_position(channel, i, swing)
  sleep(0.1)
  print(i)

'''


