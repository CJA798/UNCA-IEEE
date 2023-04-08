from NavigationSystem import NavigationSystem, NavigationStatus
from CollectionSystem import CollectionSystem, CollectionStatus


class RobotStatus:
    WAITING = 0
    NAVIGATING = 1
    COLLECT = 2
    DONE = 3
    

class Robot:
    def __init__(self) -> None:
        self.status = RobotStatus.NAVIGATING
        self.CollectionSystem = CollectionSystem()
        self.NavigationSystem = NavigationSystem()
        
