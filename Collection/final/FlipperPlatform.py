from time import sleep
import asyncio
from pi_servo_hat import PiServoHat


_ROTATION_SERVO_CHANNEL = 0
_FLIPPER_SERVO_CHANNEL = 1
#_CLEAR_SERVO_CHANNEL = 2
_STOP_ROTATION = 107
_ROTATE = 180
_FLIPPER_OFF = 240
_FLIPPER_ON = 0
_SWING = 180
_ROTATION_MIN_THRESHOLD = 50
_ROTATION_MAX_THRESHOLD = 90
_ROTATION_MEAN_THRESHOLD = int((_ROTATION_MIN_THRESHOLD + _ROTATION_MAX_THRESHOLD) / 2)


class FlipperStatus:
    EMPTY = 0
    ORIENTING = 1
    FLIPPING = 2
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
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, _STOP_ROTATION, _SWING)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, _FLIPPER_OFF, _SWING)


    async def wait(self, duration: int):
        ''' This method initiates an asynchronous timer '''
        await asyncio.sleep(duration)


    def rotate_platform(self) -> None:
        ''' This method rotates the flipper platform '''
        self.set_status(FlipperStatus.ORIENTING)
        print("Orienting")
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, 60)
        

    def stop_rotation(self) -> None:
        ''' This method stops the rotating platform '''
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, 54)


    async def flip_platform(self) -> None:
        self.set_status(FlipperStatus.FLIPPING)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 0, 180)
        await self.wait(0.5)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 240, 180)
        await self.wait(0.5)
        self.set_status(FlipperStatus.EMPTY)
        print("Platform flipped")
        

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

