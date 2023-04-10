import asyncio
from time import sleep
from pi_servo_hat import PiServoHat


_ORIENTATION_SERVO_CHANNEL = 2
_ELEVATOR_SERVO_CHANNEL = 1
_STOP_ORIENTATION = 107
_START_ORIENTATION = 180
_SWING = 180
_ROTATION_MIN_THRESHOLD = 50
_ROTATION_MAX_THRESHOLD = 90


class ElevatorStatus:
    READY = 0 #Waiting for flipper, then the camera to orient
    UNORIENTED_OBJECT = 1 #Rotate into the correct position then go to oriented
    ORIENTED_OBJECT = 3 #Rotated and can move straight into raising
    RAISING = 4 #Raising until done.
    RAISED = 5 #Done and letting drum rotate to get the object pushed in
    LOWERING = 6 #Done with being pushed and lower. Once done going into empty


class Elevator():
    def __init__(self):
        self.status = ElevatorStatus.READY
        self.current_angle = 0
        self.swing = 180
        self.hat = PiServoHat()
        # Restart Servo Hat (in case Hat is frozen/locked)
        self.hat.restart()
        # Set the PWM frequency to 50Hz
        self.hat.set_pwm_frequency(50)
        # Lower elevator on instantiation
        self.lowerToGround()
        print("Initialized Elevator")
    
    async def wait(self, duration: int):
        await asyncio.sleep(duration)

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# This is the orientation platform on the elevator
    def rotate_platform(self) -> None:
        ''' This method rotates the orientation platform '''
        self.hat.move_servo_position(_ORIENTATION_SERVO_CHANNEL, _START_ORIENTATION, _SWING)
        


    def stop_rotation(self) -> None:
        self.hat.move_servo_position(_ORIENTATION_SERVO_CHANNEL, _STOP_ORIENTATION, _SWING)

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#This is the servo movement to raise and lower the platform
    def raisePlatform(self):
        '''
        Raises the platform up to the duck location
        '''
        #The determined position of the servo for ground is -110
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, -48) # Push into Cylinder pos
        sleep(1)
        self.status = ElevatorStatus.RAISED
        

    
        

    def lowerToGround(self):
        '''
        Lowers the platform back to the base state
        '''
        #The determined position of the servo for ground is 230
        self.setStatus(ElevatorStatus.READY)
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, 115) # return home elevader pos
        sleep(1)

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




