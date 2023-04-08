import NavigationSystem
import CollectionSystem

class RobotStatus:
    WAITING = 0
    COLLECT = 1
    
class Robot:
    def __init__(self):
        self.CollectionSystem = CollectionSystem.CollectionSystem()
        self.NavigationSystem = NavigationSystem.NavigationSystem()
    
