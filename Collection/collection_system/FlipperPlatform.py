import time
import asyncio
from pi_servo_hat import PiServoHat


_ROTATION_SERVO_CHANNEL = 0
_FLIPPER_SERVO_CHANNEL = 1
_CLEAR_SERVO_CHANNEL = 2
_STOP_ROTATION = 60
_ROTATE = 54

class FlipperPlatformStatus:
    NO_OBJECTS_IN_PLATFORM = 0
    UNORIENTED_OBJECT = 1
    ORIENTED_OBJECT = 2
    FLIPPING_OBJECT = 3
    FLIPPER_PLATFORM_ERROR_MORE_THAN_ONE_OBJECT = 4
    FLIPPER_PLATFORM_ERROR_UNKNOWN = 5
    ORIENTING_OBJECT = 6


class FlipperPlatform:
    def __init__(self):
        self.status = FlipperPlatformStatus.NO_OBJECTS_IN_PLATFORM
        self.current_angle = 0
        self.num_objects = 0

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, _STOP_ROTATION)

    async def wait(self, duration: int):
        await asyncio.sleep(duration)

    def rotate_platform(self):
        self.set_status(FlipperPlatformStatus.ORIENTING_OBJECT)
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, _ROTATE)

    def stop_rotation(self):
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, _STOP_ROTATION)

    async def flip_platform(self):
        self.set_status(FlipperPlatformStatus.FLIPPING_OBJECT)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 0, 180)
        await self.wait(0.5)
        self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 240, 180)
        await self.wait(0.5)
        self.set_status(FlipperPlatformStatus.NO_OBJECTS_IN_PLATFORM)
        print("Platform flipped")


    def clear_platform(self):
        self.clear_servo.write(80)
        self.wait(2)
        self.clear_servo.write(10)
        self.wait(2)

    def get_status(self):
        return self.status

    def set_status(self, state: FlipperPlatformStatus):
        self.status = state

    def get_current_angle(self):
        return self.current_angle

    def set_current_angle(self, angle: int):
        self.current_angle = angle

    def get_num_objects(self):
        return self.num_objects

