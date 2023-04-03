import time
from pi_servo_hat import PiServoHat


_SERVO1_CHANNEL = 9
_SERVO2_CHANNEL = 10

_STOP_ROTATION_SERVO1 = 60
_ROTATE_SERVO1 = 54

''' TODO: CALIBRATE SERVO AND FIND VALUES'''
_STOP_ROTATION_SERVO2 = 60
_ROTATE_SERVO2 = 54


class IntakeStatus:
    INTAKE_ON = 0
    INTAKE_OFF = 1

class IntakeSystem:
    def __init__(self) -> None:
         # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servos on instantiation
        self.hat.move_servo_position(_SERVO1_CHANNEL, _STOP_ROTATION_SERVO1)
        self.hat.move_servo_position(_SERVO2_CHANNEL, _STOP_ROTATION_SERVO2)


    def StartServo(self, servo: int) -> None:
        ''' This method starts a single servo'''
        pass


    def StopServo(self, servo: int) -> None:
        ''' This method stops a single servo'''
        pass

        