import ElevatorPlatform
import PusherSystem
import Sweeper
import FlipperPlatform
import pi_servo_hat
from time import sleep

test = pi_servo_hat.PiServoHat()
test.restart()
test.set_pwm_frequency(50)

test.move_servo_position(5, 50)
#test.move_servo_position(2, 240, 180)
'''for i in range(0, 180, 1):
        test.move_servo_position(5, i)
        print(i)
        sleep(.1)'''
        #for i in range(-30, 0, 1):
        
