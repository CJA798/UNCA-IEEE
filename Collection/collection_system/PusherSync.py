import time
import asyncio
from pi_servo_hat import PiServoHat


_PUSHER1_SERVO_CHANNEL = 5
_PUSHER2_SERVO_CHANNEL = 6


#class FlipperPlatformStatus:
class PusherStatus:
    
    RETRACTED = 0
    
    LOADING = 1
    
    LOADED = 2
    
    RETRACTING = 3

    #HALF_UNLOADING

    #HALF_UNLOADED
   
    UNLOADING = 4
    
    UNLOADED = 5


#class FlipperPlatform:
class PusherSync:
    def __init__(self):
        PusherSync.RetractPusher(self, -110)
        self.status = PusherStatus.NO_MOVEMENT
        self.current_angle = 0
        self.num_objects = 0
        

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        
    # no async code
    # async def wait(self, duration: int):
    #    await asyncio.sleep(duration)


    #no platform only pusher
 
 
    '''
    async def activate_push1(self):
        self.set_status(PusherStatus.PUSHING_OBJECT)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, 230)
        await self.wait(5)

    async def retract_push1(self):
        self.set.status(PusherStatus.OBJECT_PUSHED)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, -110)
        await self.wait(5)
    '''


    #Pusher 1 is for pillars
    def RetractPusher(self, howFar):
        self.set.status(PusherStatus.RETRACTING)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, howFar)
        self.hat.status(PusherStatus.RETRACTED)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#loads pillar(not a powerful push)
    def LoadingPillar(self):
        self.set_status(PusherStatus.LOADING)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, 100)
        self.hat.status(PusherStatus.LOADED)
        PusherSync.RetractPusher(self, -10)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#powerful push to unload
    def UnloadingPillar(self):
        self.hat.status(PusherStatus.UNLOADING)
        self.hat.move_servo_position(_PUSHER1_SERVO_CHANNEL, 200)
        self.hat.status(PusherStatus.UNLOADED)
        PusherSync.RetractPusher(self, -110)
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