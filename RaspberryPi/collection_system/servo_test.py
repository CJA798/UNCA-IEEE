
from time import sleep
from pi_servo_hat import PiServoHat
import RPi.GPIO as GPIO
from ElevatorPlatform import Elevator
from FlipperPlatform import FlipperPlatform
from IntakeSystem import IntakeSystem


intake = IntakeSystem()
flipper = FlipperPlatform()
elevator = Elevator()

elevator.lowerToGround()

'''
intake.StartIntake()
sleep(5)
intake.StopIntake()
sleep(5)

'''
flipper.rotate_platform()
sleep(5)
flipper.FlipPlatform()
sleep(1)
flipper.FlipPlatform()
sleep(1)
flipper.FlipPlatform()
sleep(1)
flipper.FlipPlatform()
sleep(1)


sleep(5)
elevator.raisePlatformToColumn()
sleep(5)
elevator.RotatePlatform()
sleep(5)
