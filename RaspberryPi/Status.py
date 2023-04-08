class CameraStatus:
    CAMERA_OFF = 0
    CAMERA_ON = 32768

class IntakeStatus:
    INTAKE_ON = 0
    INTAKE_OFF = 8192
    INTAKE_REMOVING_JAM_CW = 16384
    INTAKE_REMOVING_JAM_CCW = 24576


class FlipperStatus:
    FLIPPER_CLEANING = 0
    FLIPPER_EMPTY = 0
    FLIPPER_ORIENTING = 2048
    FLIPPER_FLIPPING = 4096
    FLIPPER_NONE = 6144


class SweeperStatus:
    SWEEPER_RESTING = 0
    SWEEPER_EJECTING = 512
    SWEEPER_PUSHING = 1024
    SWEEPER_SUPPORTING = 1536  


class ElevatorStatus:
    ELEVATOR_CLEANING = 0
    ELEVATOR_EMPTY = 0 #Waiting for flipper, then the camera to orient
    ELEVATOR_ORIENTING = 128 #Rotating while waiting for Threshhold to be met
    ELEVATOR_RAISING = 256 #Raising until done.
    ELEVATOR_RAISED = 256 # Same as rising, just for easier to read logic
    ELEVATOR_NONE = 384


class PusherStatus:
    TOP_PUSHER_RETRACTED = 0
    TOP_PUSHER_LOADING = 32
    TOP_PUSHER_HALF_LOADING = 64
    TOP_PUSHER_UNLOADING = 96

    BOT_PUSHER_RETRACTED = 0
    BOT_PUSHER_LOADING = 8
    BOT_PUSHER_HALF_LOADING = 16
    BOT_PUSHER_UNLOADING = 24

class DrumStatus:
    pass
    '''
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

    '''
    


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
    COLLECTION_OFF = 0
    COLLECTION_COLLECTING = 1
    COLLECTION_POND_TOWER_READY = 2
    COLLECTION_SMALL_TOWER_READY = 3
    COLLECTION_DONE = 4


class NavigationStatus:
    NAVIGATION_START = 8
    NAVIGATION_ENABLE_COLLECTION = 0
    NAVIGATION_MOVING_TO_POND = 1
    NAVIGATION_IN_POND = 2
    NAVIGATION_MOVING_TO_LEFT_CIRCLE = 3
    NAVIGATION_IN_LEFT_CIRCLE = 4
    NAVIGATION_MOVING_TO_RIGHT_CIRCLE = 5
    NAVIGATION_IN_RIGHT_POND = 6
    NAVIGATION_DONE = 7

class RobotStatus():
    def __init__(self):
        self.navigation_status = NavigationStatus.NAVIGATION_START
        self.collection_status = CollectionStatus.COLLECTION_OFF
        self.camera_status = CameraStatus.CAMERA_ON
        self.intake_status = IntakeStatus.INTAKE_OFF
        self.flipper_status = FlipperStatus.FLIPPER_EMPTY
        self.sweeper_status = SweeperStatus.SWEEPER_RESTING
        self.elevator_status = ElevatorStatus.ELEVATOR_EMPTY
        self.top_pusher_status = PusherStatus.TOP_PUSHER_RETRACTED
        self.bot_pusher_status = PusherStatus.BOT_PUSHER_RETRACTED
        self.brace_status = BraceStatus.A