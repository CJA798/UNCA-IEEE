from NavigationSystem import NavigationSystem
from CollectionSystem import CollectionSystem


class RobotStatus:
    WAITING = 0
    NAVIGATING = 1
    COLLECTING = 2
    DONE = 3
    

class Robot:
    def __init__(self) -> None:
        self.status = RobotStatus.NAVIGATING
        self.CollectionSystem = CollectionSystem()
        self.NavigationSystem = NavigationSystem()
        
