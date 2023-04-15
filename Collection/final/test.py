import ElevatorPlatform
import PusherSystem
import Sweeper
import FlipperPlatform
import pi_servo_hat
from time import sleep

test = pi_servo_hat.PiServoHat()
test.restart()
test.set_pwm_frequency(50)

test.move_servo_position(6, 0)
#test.move_servo_position(2, 240, 180)
'''for i in range(0, -30, -1):
        test.move_servo_position(6, -20)
        sleep(.1)
        #for i in range(-30, 0, 1):
        test.move_servo_position(6, 0)
        sleep(.1)'''
