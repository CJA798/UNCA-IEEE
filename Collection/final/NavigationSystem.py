class NavigationSystem:
    def __init__(self)  -> None:
        self.status = NavigationStatus.WAITING_FOR_START


class NavigationStatus:
    WAITING_FOR_START = 0
    NAVIGATING = 1
    MOVING_TO_LOCATION = 2
    IN_LOCATION = 3
    