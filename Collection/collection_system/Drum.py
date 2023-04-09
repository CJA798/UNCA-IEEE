import time
import asyncio
import RPi.GPIO as GPIO

# Production installed library import
from RpiMotorLib import RpiMotorLib

GPIO_pins = (-1, -1, -1)  
direction = 2       # Direction -> White
step = 3      # Step -> Blue

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#States
class DrumStatus:
    PILLAR1 = 0
    PILLAR2 = 1
    PILLAR3 = 2
    YELLOW_DUCK1 = 3
    YELLOW_DUCK2 = 4
    PINK_DUCK = 5
    START_STATE = 6
    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The Pillar colors and empty or filled states
    FILLED = 1
    EMPTY = 0
    NO_BLOCK = -1
    WHITE = 2
    GREEN = 3
    RED = 4
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#Static Variables
MAX_OBJECTS = 12 # There are 13 objects in total, but the drum can only hold 12

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#Struct for the pillars
class PillarStruct:
    
    def __init__(self):
        self.pillarState = [DrumStatus.EMPTY, DrumStatus.EMPTY]
        #2 => White Pillar
        #3 => Green Pillar
        #4 => Red Pillar
        self.pillarColor = [DrumStatus.NO_BLOCK, DrumStatus.NO_BLOCK] 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     

class Drum():
    def __init__(self):
        self.state = DrumStatus.START_STATE
        self.numOfObjects = DrumStatus.EMPTY
        self.DrumMotor = RpiMotorLib.A4988Nema(direction, step, GPIO_pins, "DRV8825")
        self.yellowDuckSlots = [DrumStatus.EMPTY, DrumStatus.EMPTY]
        self.pinkDuckSlot = DrumStatus.EMPTY
        self.pillarSlot1 = PillarStruct()
        self.pillarSlot2 = PillarStruct()
        self.pillarSlot3 = PillarStruct()
        
        
    async def wait(self, duration: int):
        await asyncio.sleep(duration)
        
    def State_Machine(self, destination):
        match self.state:
            case DrumStatus.PILLAR1:
                #Will be a green or red pillar. The order depends on which will happen first
                ## The destination for the drum to move to.
                match destination:
                    case DrumStatus.PILLAR2:
                        #Will be only Green Pillars
                        print("State 2")
                
                    case DrumStatus.PILLAR3:
                        #Will be only White pillars
                        print("State 3")

                    case DrumStatus.YELLOW_DUCK1:
                        #Holds one duck will be the array slot 0
                        print("State 4")
                
                    case DrumStatus.YELLOW_DUCK2:
                        #Holds one duck will be array slot 1
                        print("State 5")
                
                    case DrumStatus.PINK_DUCK:
                        #Hold pink duck
                        print("State 6")
                print("State 1")
                
            case DrumStatus.PILLAR2:
                #Will be only Green Pillars
                match destination:
                    case DrumStatus.PILLAR1:
                        print("Supperrrr")
                    case DrumStatus.PILLAR3:
                        #Will be only White pillars
                        print("State 3")

                    case DrumStatus.YELLOW_DUCK1:
                        #Holds one duck will be the array slot 0
                        print("State 4")
                
                    case DrumStatus.YELLOW_DUCK2:
                        #Holds one duck will be array slot 1
                        print("State 5")
                
                    case DrumStatus.PINK_DUCK:
                        #Hold pink duck
                        print("State 6")
                print("State 2")
                
            case DrumStatus.PILLAR3:
                #Will be only White pillars
                match destination:
                    case DrumStatus.PILLAR1:
                        print("Supperrrr")
                    case DrumStatus.PILLAR2:
                        #Will be only Green Pillars
                        print("State 2")

                    case DrumStatus.YELLOW_DUCK1:
                        #Holds one duck will be the array slot 0
                        print("State 4")
                
                    case DrumStatus.YELLOW_DUCK2:
                        #Holds one duck will be array slot 1
                        print("State 5")
                
                    case DrumStatus.PINK_DUCK:
                        #Hold pink duck
                        print("State 6")
                print("State 3")
                
            case DrumStatus.YELLOW_DUCK1:
                #Holds one duck will be the array slot 0
                match destination:
                    case DrumStatus.PILLAR1:
                        print("Supperrrr")
                    case DrumStatus.PILLAR2:
                        #Will be only Green Pillars
                        print("State 2")
                
                    case DrumStatus.PILLAR3:
                        #Will be only White pillars
                        print("State 3")
                
                    case DrumStatus.YELLOW_DUCK2:
                        #Holds one duck will be array slot 1
                        print("State 5")
                
                    case DrumStatus.PINK_DUCK:
                        #Hold pink duck
                        print("State 6")
                
                print("State 4")
                
            case DrumStatus.YELLOW_DUCK2:
                #Holds one duck will be array slot 1
                match destination:
                    case DrumStatus.PILLAR1:
                        print("Supperrrr")
                    case DrumStatus.PILLAR2:
                        #Will be only Green Pillars
                        print("State 2")
                
                    case DrumStatus.PILLAR3:
                        #Will be only White pillars
                        print("State 3")

                    case DrumStatus.YELLOW_DUCK1:
                        #Holds one duck will be the array slot 0
                        print("State 4")
                    case DrumStatus.PINK_DUCK:
                        #Hold pink duck
                        print("State 6")
                print("State 5")
                
            case DrumStatus.PINK_DUCK:
                #Hold pink duck
                match destination:
                    case DrumStatus.PILLAR1:
                        print("Supperrrr")
                    case DrumStatus.PILLAR2:
                        #Will be only Green Pillars
                        print("State 2")
                
                    case DrumStatus.PILLAR3:
                        #Will be only White pillars
                        print("State 3")

                    case DrumStatus.YELLOW_DUCK1:
                        #Holds one duck will be the array slot 0
                        print("State 4")
                
                    case DrumStatus.YELLOW_DUCK2:
                        #Holds one duck will be array slot 1
                        print("State 5")
                print("State 6")
                
            case DrumStatus.START_STATE:
                #State to wait and calibrate the drum
                print("State 7")
                
    def rotate_Drum(self, clockwise: bool, step: int):
         # motor_go(clockwise, steptype, steps, stepdelay, verbose, initdelay)
        self.DrumMotor.motor_go(clockwise, "Full", step, .00000000000001, False, .05)
        
    
        
        