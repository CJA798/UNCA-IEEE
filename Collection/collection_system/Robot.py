import CollectionSystem
import NavigationSystem
#from Brace import BraceStuff. Needs to be added when Maxwell pushes


class RobotStatus:
    WAITING = 0
    COLLECT = 1
    
class Robot:
    def __init__(self):
        self.CollectionSystem = CollectionSystem()
        self.NavigationSystem = NavigationSystem()
    
