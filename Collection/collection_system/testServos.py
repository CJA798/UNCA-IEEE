import pi_servo_hat
import time

test = pi_servo_hat.PiServoHat()
test.restart()

# Moves servo position to 0 degrees (1ms), Channel 0
test.move_servo_position(13, 0)
"""
while True:
    for i in range(0, 90):
        print(i)
        test.move_servo_position(0, i)
        time.sleep(.001)
    for i in range(90, 0, -1):
        print(i)
        test.move_servo_position(0, i)
        time.sleep(.001)
"""

