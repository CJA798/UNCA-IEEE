import time
import asyncio
from pi_servo_hat import PiServoHat



#These pins can be changed depending on what is available
_HOOK_SERVO_CHANNEL = 8
_BRACE_SERVO_CHANNEL = 7


#class FlipperPlatformStatus:
class BraceStatus:
  
    OPEN = 0
   
    CLOSE = 1
   
    FILLING = 2
   
    LETGO = 3

class Brace:
    def __init__(self):
        
        self.status = BraceStatus.OPEN
        self.current_angle = 0
        self.num_objects = 0

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        Brace.BraceClosed(self)

    #async def wait(self, duration: int):
    #   await asyncio.sleep(duration)

#opens brace
    def BraceOpen(self):
        self.set_status(BraceStatus.OPEN)
        self.hat.move_servo_position(_BRACE_SERVO_CHANNEL, 180)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#closes brace
    def BraceClosed(self):
        self.set_status(BraceStatus.CLOSE)
        self.hat.move_servo_position(_BRACE_SERVO_CHANNEL, -110)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#moves hook to help with duck(not using currently) 
        #state: open or closed
        #value: from 240 to -110
    def HookActivation(self):
        self.set_status(BraceStatus.FILLING)
        self.hat.move_servo_position(_HOOK_SERVO_CHANNEL, 230)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#done task
    def CompletedTask(self):
        self.set_status(BraceStatus.LETGO)
        Brace.BraceOpen(self)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    '''async code below:
    async def initial_brace_bottom(self):
        self.set_status(BraceStatus.BRACE_BOTTOM_OPEN)
        self.hat.move_servo_position(_BRACE_BOTTOM_SERVO_CHANNEL, 230)  #not sure what value to do for open, got it at the max positive value
        await self.wait(10)
        self.set_status(BraceStatus.BRACE_BOTTOM_CLOSING)
        self.hat.move_servo_position(_BRACE_BOTTOM_SERVO_CHANNEL, 100)
        #half closed

        

    async def initial_brace_top(self):
        self.set.status(BraceStatus.BRACE_TOP_OPEN)
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, 230)
        await self.wait(10)
        #wait until intial time
        self.set.status(BraceStatus.BRACE_TOP_CLOSING)
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, -100) #not sure what the value is supposed to be


    async def tower(self, duck):
        #check for the status of cylinders to be put in
        #after 2 cylinders, have to wait for duck
        if(duck):
            #tasks for opening both braces are done meanwhile the pendulum moves
            task1 = asyncio.create_task(BRACE_ACTION.brace_open(_BRACE_TOP_SERVO_CHANNEL, BraceStatus.BRACE_TOP_OPENING))
            task2 = asyncio.create_task(BRACE_ACTION.brace_open(_BRACE_BOTTOM_SERVO_CHANNEL, BraceStatus.BRACE_BOTTOM_OPENING))

            self.set.status(BraceStatus.PENDULUM_TOP_LOCATION)
            self.hat.move_servo_position(_PENDULUM_SERVO_CHANNEL, 180)

            await self.wait(5)
            # wait for the duck to be placed then return to initial position

            BRACE_ACTION.initial_brace_bottom
            BRACE_ACTION.initial_brace_top
            BRACE_ACTION.pendulum_activation(BraceStatus.PENDULUM_BOTTOM_LOCATION, _PENDULUM_SERVO_CHANNEL, -100)
            '''


    def get_status(self):
        return self.status

    def set_status(self, state: BraceStatus):
        self.status = state

    def get_current_angle(self):
        return self.current_angle

    def set_current_angle(self, angle: int):
        self.current_angle = angle

    def get_num_objects(self):
        return self.num_objects
