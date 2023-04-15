from time import sleep
import asyncio
from pi_servo_hat import PiServoHat


''' TODO: CALIBRATE SERVO AND FIND VALUES '''
_ROTATION_SERVO_CHANNEL = 5
_FLIPPER_SERVO_CHANNEL = 6

_STOP_ORIENTATION = 50
_START_ORIENTATION = 120

_RESET_FLIPPER = 0
_FLIP_FLIPPER = 180


_SWING = 180

_ROTATION_MIN_THRESHOLD = 50
_ROTATION_MAX_THRESHOLD = 90


class FlipperStatus:
    EMPTY = 0
    ORIENTED = 1
    
    CLEAN = 3


class Flipper:
    def __init__(self) -> None:
        self.status = FlipperStatus.EMPTY
        self.current_angle = 0
        self.num_objects = 0

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servos on instantiation
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, 0)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 0)
        print("Initializing Flipper")


    async def wait(self, duration: int) -> None:
        ''' This method initiates an asynchronous timer '''
        await asyncio.sleep(duration)


    def rotate_platform(self) -> None:
        ''' This method rotates the orientation platform '''
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL,70) #subject to change
        sleep(.1)
        

    def stop_rotation(self) -> None:
        ''' This method stops the orientation platform '''
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, _STOP_ORIENTATION)


    def flip_platform(self) -> None:
        ''' This method activates the flipping platform '''
        #for i in range(0, -30, -1):
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, -60)
        sleep(.5)
        #for i in range(-30, 0, 1):
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 0)
        sleep(.1)
        
        self.set_status(FlipperStatus.EMPTY)
        

    def get_status(self) -> FlipperStatus:
        return self.status

    def set_status(self, state: FlipperStatus) -> None:
        self.status = state

    def get_current_angle(self) -> int:
        return self.current_angle

    def set_current_angle(self, angle: int) -> None:
        self.current_angle = angle

    def get_num_objects(self) -> int:
        return self.num_objects

