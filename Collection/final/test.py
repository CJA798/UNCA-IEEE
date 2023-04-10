from time import sleep
import ElevatorPlatform
import PusherSystem
import Sweeper
import FlipperPlatform
import pi_servo_hat
import time

test = pi_servo_hat.PiServoHat()
test.restart()
test.set_pwm_frequency(50)

test.move_servo_position(13, 0, 180)

'''for i in range(0, 270, 1):
        test.move_servo_position(13, i, 180)
        print(i)
        time.sleep(.5)'''