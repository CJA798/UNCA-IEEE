import time
import asyncio
from pi_servo_hat import PiServoHat



#These pins can be changed depending on what is available
_PENDULUM_SERVO_CHANNEL = 9
_BRACE_TOP_SERVO_CHANNEL = 8
_BRACE_BOTTOM_SERVO_CHANNEL = 7


#class FlipperPlatformStatus:
class BraceStatus:
  
    NO_MOVEMENT = 0
   
    BRACE_TOP_OPENING = 1
   
    BRACE_BOTTOM_OPENING = 2
   
    BRACE_TOP_CLOSING = 3
    
    BRACE_BOTTOM_CLOSING = 4

    PENDULUM_BOTTOM_LOCATION = 5

    PENDULUM_TOP_LOCATON = 6

    BRACE_TOP_OPEN = 7

    BRACE_BOTTOM_OPEN = 8


class BRACE_ACTION:
    def __init__(self):
        self.status = BraceStatus.NO_MOVEMENT
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


        def brace_open(self, pin, top_or_bottom):
            self.set.status(top_or_bottom)
            self.hat.move_servo_position(pin, 230)

        def brace_closed(self, pin, top_or_bottom):
            self.set.status(top_or_bottom)
            self.hat.move_servo_position(pin, -110)

        #state: open or closed
        #value: from 240 to -110
        def pendulum_activation(self, state, pin, value):
            self.set.status(state)
            self.hat.move_servo_position(pin, value)


    #no platform only pusher
    #
    # def rotate_platform(self):
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

    def set_status(self, state: BraceStatus):
        self.status = state

    def get_current_angle(self):
        return self.current_angle

    def set_current_angle(self, angle: int):
        self.current_angle = angle

    def get_num_objects(self):
        return self.num_objects
