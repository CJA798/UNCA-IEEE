from time import sleep
from pi_servo_hat import PiServoHat

_ROTATION_SERVO_CHANNEL = 2
_ELEVATOR_SERVO_CHANNEL = 1
_STOP_ORIENTATION = 107
_START_ORIENTATION = 120
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
    FILLED = 7
    
    
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
    def rotate_platform(self):
        self.set_status(ElevatorStatus.ORIENTED_OBJECT)
        if self.speed is not 60:
            self.speed += 1
        else: self.speed is 60
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, self.speed)
        
 


    def stop_rotation(self):
        self.speed = 54
        self.hat.move_servo_position(_ROTATION_SERVO_CHANNEL, self.speed)
    

##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#This is the servo movement to raise and lower the platform
    def raisePlatform(self):
        '''
        Raises the platform up to the duck location
        '''
        #The determined position of the servo for ground is -110
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, -55) # Push into Cylinder pos
        sleep(1)
        self.status = ElevatorStatus.RAISED
        

    def lowerToGround(self):
        '''
        Lowers the platform back to the base state
        '''
        #The determined position of the servo for ground is 230
        self.hat.move_servo_position(_ELEVATOR_SERVO_CHANNEL, 115) # return home elevader pos
        sleep(1)
        self.setStatus(ElevatorStatus.READY)

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




