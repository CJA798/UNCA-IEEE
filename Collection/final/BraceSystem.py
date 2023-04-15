from time import sleep
import asyncio
from pi_servo_hat import PiServoHat


#These pins can be changed depending on what is available
_HOOK_SERVO_CHANNEL = 11#8
_BRACE_SERVO_CHANNEL = 12#7


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
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Set initial brace position on instantiation
        self.BraceInit()

    #async def wait(self, duration: int):
    #   await asyncio.sleep(duration)

#opens brace
    def BraceShake(self):
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, 0)
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL , 75)
        self.hat.sleep(0.5)
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, 0)
        self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, 75)
        self.hat.sleep(0.5)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#closes brace
    def BraceActive(self):
        for i in range(120, 0, -1):
            self.hat.move_servo_position(8, i)
            sleep(.005)
        for i in range(52, 2, -1):
            self.hat.move_servo_position(7, i)
            sleep(.005)
        for i in range(135, 10, -1):
            self.hat.move_servo_position(9, i)
            sleep(.005)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#moves hook to help with duck(not using currently) 
        #state: open or closed
        #value: from 240 to -110
    def BraceInit(self):
        #for i in range(10, 135, 1):
            self.hat.move_servo_position(_BRACE_TOP_SERVO_CHANNEL, 0)
            sleep(.005)
        #for i in range(0, 53, 1):
            self.hat.move_servo_position(_BRACE_BOTTOM_SERVO_CHANNEL, 125)
            sleep(.005)
        #for i in range(0, 118, 1):
            self.hat.move_servo_position(_HOOK_SERVO_CHANNEL, 17)
            sleep(.005)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#done task
    def twoTowerSet(self):
        for i in range(10, 135, 1):
            self.hat.move_servo_position(9, i)
            sleep(.005)
        for i in range(0, 53, 1):
            self.hat.move_servo_position(7, i)
            sleep(.005)
        for i in range(0, 118, 1):
            self.hat.move_servo_position(8, i)
            sleep(.005)
        for i in range(135, 10, -1):
            self.hat.move_servo_position(9, i)
            sleep(.005)

# has to be in twoTowerSet to work
    def twoTowerDrop(self):
        for i in range(10, 135, 1):
            self.hat.move_servo_position(12, i)
            sleep(.005)

    def twoTowerClose(self):
        for i in range(50, 10, -1): #135 instead of 50
            self.hat.move_servo_position(11, i) #this opens the bottom
            sleep(.005)

    def threeTowerDrop(self):
        for i in range(10, 135, 1):
            self.hat.move_servo_position(9, i)
            sleep(.005)
        for i in range(0, 118, 1):
            self.hat.move_servo_position(8, i)
            sleep(.005)

    def threeTowerClose(self):
        for i in range(120, 0, -1):
            self.hat.move_servo_position(8, i)
            sleep(.005)
        for i in range(135, 10, -1):
            self.hat.move_servo_position(9, i)
            sleep(.005)
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
        #wait until intial         self.set.status(BraceStatus.BRACE_TOP_CLOSING)
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
