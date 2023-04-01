import time
import asyncio
from pi_servo_hat import PiServoHat


_PUSHER1_SERVO_CHANNEL = 5
_PUSHER2_SERVO_CHANNEL = 6


#class FlipperPlatformStatus:
class PusherStatus:
    #NO_OBJECTS_IN_PLATFORM = 0
    NO_MOVEMENT = 0
    #UNORIENTED_OBJECT = 1
    UNPUSHED_OBJECT = 1
    #ORIENTED_OBJECT = 2
    PUSHING_OBJECT = 2
    #FLIPPING_OBJECT = 3
    #FLIPPER_PLATFORM_ERROR_MORE_THAN_ONE_OBJECT = 4
    PUSHING_OBJECT_ERROR = 3
    #FLIPPER_PLATFORM_ERROR_UNKNOWN = 5
    PUSHING_OBJECT_ERROR_UNKNOWN = 4
    #ORIENTING_OBJECT = 6
    OBJECT_PUSHED = 5


#class FlipperPlatform:
class Pusher:
    def __init__(self):
        self.status = PusherStatus.NO_MOVEMENT
        self.current_angle = 0
        self.num_objects = 0

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        

    async def wait(self, duration: int):
        await asyncio.sleep(duration)


    #no platform only pusher
    #
    # def rotate_platform(self):
    async def activate_push1(self):
        self.set_status(PusherStatus.PUSHING_OBJECT)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, 230)
        await self.wait(5)



    async def retract_push1(self):
        self.set.status(PusherStatus.OBJECT_PUSHED)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, -110)
        await self.wait(5)




    async def activate_push2(self):
        self.set_status(PusherStatus.PUSHING_OBJECT)
        self.hat.move_servo_position(_PUSHER2_SERVO_CHANNEL, 230)
        await self.wait(5)

    async def retract_push2(self):
        self.set.status(PusherStatus.OBJECT_PUSHED)
        self.hat.move_servo_position(_PUSHER2_SERVO_CHANNEL, -110)
        await self.wait(5)



   # async def flip_platform(self):
    #    self.set_status(PusherStatus.FLIPPING_OBJECT)
    #    self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 0, 180)
    #    await self.wait(0.5)
    #    self.hat.move_servo_position(_FLIPPER_SERVO_CHANNEL, 240, 180)
    #    await self.wait(0.5)
    #    self.set_status(FlipperPlatformStatus.NO_OBJECTS_IN_PLATFORM)
    #    print("Platform flipped")


    #def clear_platform(self):
    #    self.wait(2)
    #    self.clear_servo.write(10)
    #    self.wait(2)

    def get_status(self):
        return self.status

    def set_status(self, state: PusherStatus):
        self.status = state

    def get_current_angle(self):
        return self.current_angle

    def set_current_angle(self, angle: int):
        self.current_angle = angle

    def get_num_objects(self):
        return self.num_objects

