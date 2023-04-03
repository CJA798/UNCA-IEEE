from time import sleep
from pi_servo_hat import PiServoHat


_INTAKE_SERVO1_CHANNEL = 9
_INTAKE_SERVO2_CHANNEL = 10

_STOP_INTAKE_SERVO1 = 107
_ROTATE_INTAKE_SERVO1 = 180
_REVERSE_INTAKE_SERVO1 = 0

''' TODO: CALIBRATE SERVO AND FIND VALUES'''
_STOP_INTAKE_SERVO2 = 107
_ROTATE_INTAKE_SERVO2 = 180
_REVERSE_INTAKE_SERVO2 = 0

_SWING = 180


class IntakeStatus:
    INTAKE_ON = 0
    INTAKE_OFF = 1
    REMOVING_JAM = 2

class IntakeSystem:
    def __init__(self) -> None:
         # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servos on instantiation
        self.hat.move_servo_position(_INTAKE_SERVO1_CHANNEL, _STOP_INTAKE_SERVO1, _SWING)
        self.hat.move_servo_position(_INTAKE_SERVO2_CHANNEL, _STOP_INTAKE_SERVO2, _SWING)


    def StartIntake(self) -> None:
        ''' This method starts the intake system '''
        print("Starting Intake")
        self.hat.move_servo_position(_INTAKE_SERVO1_CHANNEL, _ROTATE_INTAKE_SERVO1, _SWING)
        self.hat.move_servo_position(_INTAKE_SERVO2_CHANNEL, _ROTATE_INTAKE_SERVO2, _SWING)


    def StopIntake(self) -> None:
        ''' This method stops the intake system '''
        print("Stopping Intake")
        self.hat.move_servo_position(_INTAKE_SERVO1_CHANNEL, _STOP_INTAKE_SERVO1, _SWING)
        self.hat.move_servo_position(_INTAKE_SERVO2_CHANNEL, _STOP_INTAKE_SERVO2, _SWING)

    
    def RemoveJam(self) -> None:
        ''' This method attempts to remove a jam by spinning servos in
            opposite directions '''
        print("Removing jam")
        self.hat.move_servo_position(_INTAKE_SERVO1_CHANNEL, _ROTATE_INTAKE_SERVO1, _SWING)
        self.hat.move_servo_position(_INTAKE_SERVO2_CHANNEL, _REVERSE_INTAKE_SERVO2, _SWING)
        sleep(2.5)
        print("Removing jam")
        self.hat.move_servo_position(_INTAKE_SERVO1_CHANNEL, _REVERSE_INTAKE_SERVO1, _SWING)
        self.hat.move_servo_position(_INTAKE_SERVO2_CHANNEL, _ROTATE_INTAKE_SERVO2, _SWING)
        sleep(2.5)
        print("Jam removed")
        self.StopIntake()