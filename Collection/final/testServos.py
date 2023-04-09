import pi_servo_hat
import time

test = pi_servo_hat.PiServoHat()
test.restart()


# Moves servo position to 0 degrees (1ms), Channel 0
# test.move_servo_position(0, 0)

# flips it once
def flipIt():
    for i in range(0, -20, -1):
        test.move_servo_position(13, i)
        time.sleep(.01)
    for i in range(-20, 0, 1):
        test.move_servo_position(13, i)
        time.sleep(.01)

def elevatorUp():
    test.move_servo_position(1, -48) # Push into Cylinder pos
    time.sleep(1)
def elevatorDown():
    test.move_servo_position(1, 115) # return home elevader pos
    time.sleep(1)

def kickOff():
    test.move_servo_position(0, 0) # fast kicking
    time.sleep(5)
    test.move_servo_position(0, 50)

# let me know when you are using this function, requires tweaking 
def pushOnElevator():
    test.move_servo_position(0, 80) # fast-ish pushing
    time.sleep(1.5)
    test.move_servo_position(0, 50) # stops
    time.sleep(1)
    test.move_servo_position(0, 0)
    time.sleep(.77)
    test.move_servo_position(0, 50)
    time.sleep(1)

def backReady():
    for i in range(120, 0, -1):
        test.move_servo_position(8, i)
        time.sleep(.005)
    for i in range(52, 2, -1):
        test.move_servo_position(7, i)
        time.sleep(.005)
    for i in range(135, 10, -1):
        test.move_servo_position(9, i)
        time.sleep(.005)

def backInit():
    for i in range(10, 135, 1):
        test.move_servo_position(9, i)
        time.sleep(.005)
    for i in range(0, 53, 1):
        test.move_servo_position(7, i)
        time.sleep(.005)
    for i in range(0, 118, 1):
        test.move_servo_position(8, i)
        time.sleep(.005)

def shakeBack():
    test.move_servo_position(9, 10)
    test.move_servo_position(8, 5)
    time.sleep(0.5)
    test.move_servo_position(8, 0)
    test.move_servo_position(9, 5)
    time.sleep(0.5)

def twoTowerSet():
    for i in range(10, 135, 1):
        test.move_servo_position(9, i)
        time.sleep(.005)
    for i in range(0, 53, 1):
        test.move_servo_position(7, i)
        time.sleep(.005)
    for i in range(0, 118, 1):
        test.move_servo_position(8, i)
        time.sleep(.005)
    for i in range(135, 10, -1):
        test.move_servo_position(9, i)
        time.sleep(.005)

# has to be in twoTowerSet to work
def twoTowerDrop():
    for i in range(10, 135, 1):
        test.move_servo_position(9, i)
        time.sleep(.005)

def twoTowerClose():
    for i in range(135, 10, -1):
        test.move_servo_position(9, i)
        time.sleep(.005)

def threeTowerDrop():
    for i in range(10, 135, 1):
        test.move_servo_position(9, i)
        time.sleep(.005)
    for i in range(0, 118, 1):
        test.move_servo_position(8, i)
        time.sleep(.005)

def threeTowerClose():
    for i in range(120, 0, -1):
        test.move_servo_position(8, i)
        time.sleep(.005)
    for i in range(135, 10, -1):
        test.move_servo_position(9, i)
        time.sleep(.005)


