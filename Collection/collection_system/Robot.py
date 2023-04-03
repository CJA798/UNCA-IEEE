from IntakeSystem import IntakeSystem, IntakeStatus
from FlipperPlatform import FlipperPlatform, FlipperPlatformStatus
from Drum import Drum, DrumStatus
from ElevatorPlatform import Elevator, ElevatorStatus
from PusherSync import PusherSync, PusherStatus
from Brace import Brace, BraceStatus
#from Brace import BraceStuff. Needs to be added when Maxwell pushes


class RobotStatus:
    WAITING = 0
    COLLECT = 1
    
class Robot:
    def __init__(self):
        self.CollectionSystem = CollectionSystem()
        self.NavigationSystem = None
    
class CollectionSystem:
    def __init__(self):
        self.Intake = IntakeSystem()
        self.Flipper = FlipperPlatform()
        self.Drum = Drum()
        self.Elevator = Elevator()
        self.Pushers = PusherSync()
        self.Brace = Brace()
        #Brace stuff
        