import time
import asyncio
from pi_servo_hat import PiServoHat
import RPi.GPIO as GPIO

# Production installed library import
from RpiMotorLib import RpiMotorLib

GPIO_pins = (-1, -1, -1)  
direction = 16       # Direction -> White
step = 26      # Step -> Blue
enable = 14
GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD  
GPIO.setup(6, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(enable, GPIO.OUT)



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#States
#For the presentation we will only be using three of these slots
class DrumStatus:
    SLOT1 = 0 #This is column 1
    SLOT2 = 1 #Not used
    SLOT3 = 2 #Not used
    SLOT4 = 3 #Not used
    SLOT5 = 4 #Used for the single yellow duck
    SLOT6 = 5 #Used for column 2
    SLOT1OUT = 6
    SLOT2OUT = 7
    SLOT3OUT = 8 
    SLOT4OUT = 9
    SLOT5OUT = 10 
    SLOT6OUT = 11
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The Pillar colors and empty or filled states
    FILLED = 1
    EMPTY = 0
    NO_BLOCK = -1
    YELLOW_DUCK = 0
    PINK_DUCK = 1
    WHITE = 2
    GREEN = 3
    RED = 4
    MAX_OBJECTS = 12 # There are 13 objects in total, but the drum can only hold 12
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#Struct for the pillars
class PillarStruct:
    
    def __init__(self):
        self.pillarState = [DrumStatus.EMPTY, DrumStatus.EMPTY]
        #2 => White Pillar
        #3 => Green Pillar
        #4 => Red Pillar
        #self.pillarColor = [DrumStatus.NO_BLOCK, DrumStatus.NO_BLOCK] 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     
#Initializing Drum
class Drum():
    def __init__(self):
        self.state = DrumStatus.SLOT1
        self.numOfObjects = DrumStatus.EMPTY
        self.DrumMotor = RpiMotorLib.A4988Nema(direction, step, GPIO_pins, "DRV8825")
        self.Slot5Status = DrumStatus.EMPTY #This is the yellow Duck
        self.Slot1Status = DrumStatus.EMPTY #This is the first column
        self.Slot6Status = DrumStatus.EMPTY #This is the second column
        GPIO.output(enable, 1)
        #self.calibrateDrum()
        '''self.yellowDuckSlots = [DrumStatus.EMPTY, DrumStatus.EMPTY]
        self.pinkDuckSlot = DrumStatus.EMPTY
        self.pillarSlot1 = PillarStruct()
        self.pillarSlot2 = PillarStruct()
        self.pillarSlot3 = PillarStruct()
        This will be used when we decide the final setup for the drum'''
        
    
    def calibrateDrum(self):
        try:
            while True:
                if GPIO.input(6):
                    #self.DrumMotor.stop_motor()
                    break
                else:
                    self.DrumMotor.motor_go(True, "Full", 1, .000000001, False, .0001)
                    print(GPIO.input(6))
        finally:    
            GPIO.output(14, 0)
        
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    async def wait(self, duration: int):
        await asyncio.sleep(duration)
        
    def stateMachineInput(self, destination):
        switch = {
            DrumStatus.SLOT1: {
                DrumStatus.SLOT1OUT: lambda: self.rotate_Drum(True, 20623)
            }
        }

        switch.get(self.state, {}).get(destination, lambda: None)()
            
                
    def rotate_Drum(self, clockwise: bool, step: int):
         # motor_go(clockwise, steptype, steps, stepdelay, verbose, initdelay)
        self.DrumMotor.motor_go(clockwise, "Full", step, .00000000000001, False, .05)
        #self.DrumMotor.stop_motor()
        
    
        
        
