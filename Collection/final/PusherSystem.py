import time
import asyncio
from pi_servo_hat import PiServoHat

#call Pusher1 top and Pusher2 bottom(for Dennis)
_PUSHER1_SERVO_CHANNEL = 5
_PUSHER2_SERVO_CHANNEL = 6


#class FlipperPlatformStatus:
class PusherStatus:
    RETRACTED = 0
    LOADING = 1
    LOADED = 2
    RETRACTING = 3
    UNLOADING = 4
    UNLOADED = 5


#class FlipperPlatform:
class Pushers:
    def __init__(self):
        
        self.status = PusherStatus.RETRACTED
        self.current_angle = 0
        self.num_objects = 0
        

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        self.RetractPusher(self,_PUSHER1_SERVO_CHANNEL, -110)
        self.RetractPusher(self,_PUSHER2_SERVO_CHANNEL, -110)
    # no async code
    # async def wait(self, duration: int):
    #    await asyncio.sleep(duration)


    #no platform only pusher


    #Pusher 1 is for pillars
    def RetractPusher(self, pin, howFar):
        self.set_status(PusherStatus.RETRACTING)
        self.hat.move_servo_position(pin, howFar)
        self.set_status(PusherStatus.RETRACTED)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#loads pillar(not a powerful push)
    def LoadingPillarPusher1(self):
        self.set_status(PusherStatus.LOADING)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, 100)
        self.set_status(PusherStatus.LOADED)
        Pushers.RetractPusher(self, _PUSHER1_SERVO_CHANNEL, -10)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#powerful push to unload
    def UnloadingPillarPusher2(self):
        self.set_status(PusherStatus.UNLOADING)
        self.hat.move_servo_position(_PUSHER2_SERVO_CHANNEL, 200)
        self.set_status(PusherStatus.UNLOADED)
        Pushers.RetractPusher(self, _PUSHER2_SERVO_CHANNEL, -110)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



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