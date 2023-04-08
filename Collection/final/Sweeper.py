from time import sleep
from pi_servo_hat import PiServoHat


''' TODO: CALIBRATE SERVO AND FIND VALUES'''
_SWEEPER_SERVO_CHANNEL = 12
_ROTATE_SWEEPER = 180
_STOP_SWEEPER = 107

_SWING = 180


class SweeperStatus:
    RESTING = 0
    PUSHING = 1
    EJECTING = 2
    SUPPORTING = 3

class Sweeper:
    def __init__(self) -> None:
        self.status = SweeperStatus.RESTING
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servo on instantiation
        self.hat.move_servo_position(_SWEEPER_SERVO_CHANNEL, _STOP_SWEEPER, _SWING)

    ''' TODO: ADD ALL THE SWEEPER FUNCTIONS '''
    def reset_sweeper(self) -> None:
        ''' This method resets the sweeper position to the default resting position '''
        print("Resting Sweeper")
        self.hat.move_servo_position(_SWEEPER_SERVO_CHANNEL, _STOP_SWEEPER, _SWING)


    def stop_sweeper(self) -> None:
        ''' This method stops the sweeper '''
        print("Stopping Sweeper")
        self.hat.move_servo_position(_SWEEPER_SERVO_CHANNEL, _STOP_SWEEPER, _SWING)


    def sweep(self) -> None:
        ''' This method runs the sweeping sequence '''
        print("Stopping Sweeper")


    def push(self) -> None:
        ''' This method starts the pushing sequence from flipper to elevator '''
        print("Pushing to Elevator")
