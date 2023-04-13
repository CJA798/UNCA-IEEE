import asyncio
from pi_servo_hat import PiServoHat

class ElevatorStatus:
    READY = 0 #Waiting for flipper, then the camera to orient
    UNORIENTED_OBJECT = 1 #Rotate into the correct position then go to oriented
    ORIENTED_OBJECT = 2 #Waiting for the camera to tell me to move to the height of an object
    DUCK = 3
    COLUMN = 4
    DONE = 5 #Go back down to the ready state
    CLEANING = 6 #The orientation isn't fixable, so cleaning
_ROTATION_SERVO_CHANNEL = 3
_ELEVATOR_SERVO_CHANNEL = 4

class Elevator():
    def __init__(self):
        self.status = ElevatorStatus.READY
        self.current_angle = 0
        self.num_objects = 0
        self.swing = 180
        self.speed = 54
        

        # Instantiate the object
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Save the hat object as an attribute
    
    async def wait(self, duration: int):
        await asyncio.sleep(duration)

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# This is the orientation platform on the elevator
    def rotate_platform(self):
        self.set_status(ElevatorStatus.ORIENTED_OBJECT)
        if self.speed is not 60:
            self.speed += 1
        else: self.speed is 0
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, self.speed)
        
 


    def stop_rotation(self):
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, 54)
    

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#This is the servo movement to raise and lower the platform
    async def raisePlatformToDuck(self):
        #The determined position of the servo for ground is -110
        self.setStatus(ElevatorStatus.DUCK)
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, -110, self.swing)
        await self.wait(0.5)

    async def raisePlatformToColumn(self):
        #The determined position of the servo for ground is 0
        self.setStatus(ElevatorStatus.COLUMN)
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, 0, self.swing)
        await self.wait(0.5)

    async def lowerToGround(self):
        #The determined position of the servo for ground is 230
        self.setStatus(ElevatorStatus.DONE)
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, 230, self.swing)
        await self.wait(0.5)

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
## Getters and Setters
    def setStatus(self, state: ElevatorStatus):
        self.status = state
    def getStatus(self):
        return self.status
    def getAngle(self):
        return self.current_angle
    def set_current_angle(self, angle: int):
        self.current_angle = angle
    def get_num_objects(self):
        return self.num_objects




