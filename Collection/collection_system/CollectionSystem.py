import IntakeSystem
import FlipperPlatform
import ElevatorPlatform
import PusherSync
import Drum
import Brace


class CollectionStatus:
    _WAITING_FOR_TEENSY = 0
    _POND_TOWER_READY = 99
    _LEFT_TOWER_READY = 98
    _RIGHT_TOWER_READY = 97


class CollectionSystem:
    def __init__(self):
        self.Intake = IntakeSystem()
        self.Flipper = FlipperPlatform()
        self.Drum = Drum()
        self.Elevator = ElevatorPlatform()
        self.Pushers = PusherSync()
        self.Brace = Brace()