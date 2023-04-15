from IntakeSystem import IntakeSystem
from FlipperPlatform import Flipper
from ElevatorPlatform import Elevator
from PusherSystem import Pushers
from BraceSystem import Brace
from Sweeper import Sweeper


class CollectionSystem:
    def __init__(self) -> None:
        self.status = CollectionStatus.COLLECTING
        self.Intake = IntakeSystem()
        self.Flipper = Flipper()
        
        #self.Brace = Brace()
        self.Sweep = Sweeper()
        self.Pushers = Pushers()
        self.Elevator = Elevator()


class CollectionStatus:
    OFF = 0
    COLLECTING = 1
    GO_TO_BIG_TOWER = 2
    GO_TO_LEFT_TOWER = 3
    GO_TO_RIGHT_TOWER = 4
    WAITING_FOR_RELOCATION = 5