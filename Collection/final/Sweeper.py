from time import sleep
from pi_servo_hat import PiServoHat


''' TODO: CALIBRATE SERVO AND FIND VALUES'''
_SWEEPER_IN_SERVO_CHANNEL = 2
_SWEEPER_OUT_SERVO_CHANNEL = 11



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
        self.reset_sweeper()

    ''' TODO: ADD ALL THE SWEEPER FUNCTIONS '''
    def reset_sweeper(self) -> None:
        ''' This method resets the sweeper position to the default resting position '''
        print("Resting Sweeper")
        self.hat.move_servo_position(_SWEEPER_IN_SERVO_CHANNEL, -40, 180)
        sleep(1)


    def stop_sweeper(self) -> None:
        ''' This method stops the sweeper '''
        print("Stopping Sweeper")
        self.hat.move_servo_position(_SWEEPER_OUT_SERVO_CHANNEL, 0)


    def sweep(self) -> None:
        ''' This method runs the sweeping sequence '''
        self.hat.move_servo_position(_SWEEPER_OUT_SERVO_CHANNEL, 0) # fast kicking
        sleep(1)
        self.hat.move_servo_position(_SWEEPER_OUT_SERVO_CHANNEL, 50)


    def push(self) -> None:
        ''' This method starts the pushing sequence from flipper to elevator '''
        self.hat.move_servo_position(_SWEEPER_IN_SERVO_CHANNEL, 240, 180) # fast-ish pushing
        print("I pushed the ducky")
        sleep(1)
        self.reset_sweeper()




        '''
                sleep(1.5)
        self.hat.move_servo_position(0, 50) # stops
        sleep(1)
        self.hat.move_servo_position(0, 0)
        sleep(.77)
        self.hat.move_servo_position(0, 50)
        sleep(1)

        '''
