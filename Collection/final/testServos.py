import pi_servo_hat
import time

test = pi_servo_hat.PiServoHat()
test.restart()


# Moves servo position to 0 degrees (1ms), Channel 0
# test.move_servo_position(0, 0)

# flips it once
def flipIt():
    for i in range(2, -20, -1):
        test.move_servo_position(0, i)
        time.sleep(.1)
    for i in range(-20, 2, 1):
        test.move_servo_position(0, i)
        time.sleep(.1)

def kickOff():
    test.move_servo_position(1, 0) # fast kicking
    time.sleep(5)
    test.move_servo_position(1, 50)

def pushOnElevator():
    test.move_servo_position(1, 80) # fast-ish pushing
    time.sleep(1.5)
    test.move_servo_position(1, 50) # stops
    time.sleep(1)
    test.move_servo_position(1, 0)
    time.sleep(.77)
    test.move_servo_position(1, 50)
    time.sleep(1)

# test.move_servo_position(2, -48) # Push into Cylinder pos

# test.move_servo_position(2, 112) # return home elevader pos

test.move_servo_position(3, 90)