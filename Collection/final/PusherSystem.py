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
        self.status = PusherStatus.RETRACTED
        self.current_angle = 0
        self.num_objects = 0
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Stop servos on instantiation
        self.RetractPusher(self, _TOP_PUSHER_SERVO_CHANNEL, -110)
        self.RetractPusher(self, _BOT_PUSHER_SERVO_CHANNEL, -110)

    #Pusher 1 is for pillars
    def RetractPusher(self, pin, howFar):
        ''' This method retracts a specific pusher a certain distance '''
        self.set_status(PusherStatus.RETRACTING)
        print("Retracting Pusher {} this ammount: {}".format(pin, howFar))
        self.hat.move_servo_position(pin, howFar)
        self.set_status(PusherStatus.RETRACTED)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#loads pillar(not a powerful push)
    def LoadingPillarPusher1(self) -> None:
        ''' This method uses the bottom pusher to load an object '''
        self.set_status(PusherStatus.LOADING)
        print("Loading Item")
        self.hat.move_servo_position(_TOP_PUSHER_SERVO_CHANNEL, 100)
        self.set_status(PusherStatus.LOADED)
        Pushers.RetractPusher(self, _TOP_PUSHER_SERVO_CHANNEL, -10)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#powerful push to unload pillars and pink duck
    def UnloadingPillarPusher2(self) -> None:
        ''' This method uses the top pusher to unload pillars or the pink duck '''
        self.set_status(PusherStatus.UNLOADING)
        print("Unloading Pillar or Pink Duck")
        self.hat.move_servo_position(_BOT_PUSHER_SERVO_CHANNEL, 200)
        self.set_status(PusherStatus.UNLOADED)
        Pushers.RetractPusher(self, _BOT_PUSHER_SERVO_CHANNEL, -110)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#powerful push to unload Ducks
    def UnloadingYellowDuck(self) -> None:
        ''' This method uses the bottom pusher to unload a yellow duck '''
        self.set_status(PusherStatus.UNLOADING)
        print("Unloading Yellow Duck")
        self.hat.move_servo_position(_TOP_PUSHER_SERVO_CHANNEL, 200)
        self.set_status(PusherStatus.UNLOADED)
        Pushers.RetractPusher(self, _TOP_PUSHER_SERVO_CHANNEL, -110)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    def get_status(self) -> PusherStatus:
        return self.status

    def set_status(self, state: PusherStatus)  -> None:
        self.status = state