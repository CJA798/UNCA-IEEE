class IntakeStatus:
    INTAKE_ON = 0
    INTAKE_OFF = 1
    REMOVING_JAM_CW = 2
    REMOVING_JAM_CCW = 3



class FlipperStatus:
    EMPTY = 0
    ORIENTING = 1
    FLIPPING = 2
    NONE = 3

    CLEANING = 0


class SweeperStatus:
    EJECTING = 0
    PUSHING = 1
    SUPPORTING = 2
    RESTING = 3


class ElevatorStatus:
    EMPTY = 0 #Waiting for flipper, then the camera to orient
    ORIENTING = 1 #Rotating while waiting for Threshhold to be met
    RAISING = 2 #Raising until done.
    NONE = 3

    CLEANING = 0


class PusherStatus:
    RETRACTED = 0
    LOADING = 1
    HALF_LOADING = 2
    UNLOADING = 3


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


class BraceStatus:
    A = 0
    B = 1
    C = 2
    D = 3
    E = 4
    F = 5
    G = 6
    H = 7


class CollectionStatus:
    OFF = 0
    COLLECTING = 1
    POND_TOWER_READY = 2
    SMALL_TOWER_READY = 3
    DONE = 4


class NavigationStatus:
    START = 8
    ENABLE_COLLECTION = 0
    MOVING_TO_POND = 1
    IN_POND = 2
    MOVING_TO_LEFT_CIRCLE = 3
    IN_LEFT_CIRCLE = 4
    MOVING_TO_RIGHT_CIRCLE = 5
    IN_RIGHT_POND = 6
    DONE = 7

class RobotStatus():
    def __init__(self):
        self.navigation_status = NavigationStatus.START
        self.collection_status = CollectionStatus.OFF
        self.intake_status = IntakeStatus.INTAKE_OFF
        self.flipper_status = FlipperStatus.EMPTY
        self.sweeper_status = SweeperStatus.RESTING
        self.elevator_status = ElevatorStatus.EMPTY
        self.top_pusher_status = PusherStatus.UNLOADING
        self.bot_pusher_status = PusherStatus.UNLOADING
        self.brace_status = BraceStatus.A