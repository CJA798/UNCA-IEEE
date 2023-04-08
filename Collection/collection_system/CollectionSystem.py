from IntakeSystem import IntakeSystem, IntakeStatus
from FlipperPlatform import FlipperPlatform, FlipperStatus
from ElevatorPlatform import Elevator, ElevatorStatus
from PusherSync import PusherSync, PusherStatus
from Brace import Brace, BraceStatus

class CollectionSystem:
    def __init__(self):
        self.status = CollectionStatus.FLIPPER
        self.Intake = IntakeSystem()
        self.Flipper = FlipperPlatform()
        self.Elevator = Elevator()
        self.Pushers = PusherSync()
        self.Brace = Brace()


class CollectionStatus:
    OFF = 0
    COLLECTING = 1
    GO_TO_BIG_TOWER = 2
    GO_TO_LEFT_TOWER = 3
    GO_TO_RIGHT_TOWER = 4