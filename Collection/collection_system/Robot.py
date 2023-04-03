from FlipperPlatform import FlipperPlatform, FlipperPlatformStatus
from Drum import Drum, DrumStatus
from ElevatorPlatform import Elevator, ElevatorStatus
from Pusher import Pusher, PusherStatus
from BraceCode import BRACE_ACTION
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
        self.Flipper = FlipperPlatform()
        self.Drum = Drum()
        self.Elevator = Elevator()
        self.Pushers = Pusher()
        self.Brace = BRACE_ACTION()
        #Brace stuff
        