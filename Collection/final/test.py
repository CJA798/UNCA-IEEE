import ElevatorPlatform
import PusherSystem
import Sweeper
import FlipperPlatform
import pi_servo_hat
import time

test = pi_servo_hat.PiServoHat()
test.restart()
test.set_pwm_frequency(50)

#test.move_servo_position(5, -110, 180)

for i in range(0, 270, 1):
    test.move_servo_position(5, i)
    print(i)
    time.sleep(0.1)