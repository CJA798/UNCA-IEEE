import time
import asyncio
from pi_servo_hat import PiServoHat


''' TODO: CALIBRATE SERVO AND FIND VALUES'''
#call Pusher1 top and Pusher2 bottom(for Dennis)
_TOP_PUSHER_SERVO_CHANNEL = 5
_BOT_PUSHER_SERVO_CHANNEL = 6


#class FlipperPlatformStatus:
class PusherStatus:
    RETRACTED = 0
    LOADING = 1
    LOADED = 2
    RETRACTING = 3
    UNLOADING = 4
    UNLOADED = 5
    READY = 6


#class FlipperPlatform:
class Pushers:
    def __init__(self):      
        self.statusTop = PusherStatus.RETRACTED
        self.statusBot = PusherStatus.RETRACTED
        
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servos on instantiation
        self.RetractPusherTop()
        self.RetractPusherBot()

    #Pusher 1 is for pillars
    def RetractPusherTop(self):
        ''' This method retracts a specific pusher a certain distance '''
        self.hat.move_servo_position(_TOP_PUSHER_SERVO_CHANNEL, -110)
        self.statusTop = PusherStatus.RETRACTED
        
    def RetractPusherBot(self):
        ''' This method retracts a specific pusher a certain distance '''
        self.hat.move_servo_position(_BOT_PUSHER_SERVO_CHANNEL, -110)
        self.statusBot = PusherStatus.RETRACTED
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#loads pillar(not a powerful push)
    def LoadingPillarPusher(self) -> None:
        ''' This method uses the bottom pusher to load an object '''
        self.hat.move_servo_position(_BOT_PUSHER_SERVO_CHANNEL, 100)
        
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#powerful push to unload pillars and pink duck
    def UnloadingPillarPusherBot(self) -> None:
        ''' This method uses the top pusher to unload pillars or the pink duck '''
        self.hat.move_servo_position(_BOT_PUSHER_SERVO_CHANNEL, 200)
        self.statusBot = PusherStatus.UNLOADED
        
        
    def UnloadingPillarPusherTop(self) -> None:
        ''' This method uses the top pusher to unload pillars or the pink duck '''
        self.hat.move_servo_position(_TOP_PUSHER_SERVO_CHANNEL, 200)
        self.statusTop = PusherStatus.UNLOADED
       
        
    def Half_UnloadingPillarPusherBot(self) -> None:
        ''' This method uses the top pusher to unload pillars or the pink duck '''
        self.hat.move_servo_position(_BOT_PUSHER_SERVO_CHANNEL, 150)
        self.statusBot = PusherStatus.UNLOADED
        
        
    def Half_UnloadingPillarPusherTop(self) -> None:
        ''' This method uses the top pusher to unload pillars or the pink duck '''
        self.hat.move_servo_position(_TOP_PUSHER_SERVO_CHANNEL, 150)
        self.statusTop = PusherStatus.UNLOADED
        
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    def get_status(self) -> PusherStatus:
        return self.status

    def set_status(self, state: PusherStatus)  -> None:
        self.status = state