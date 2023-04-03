import serial


class NavigationStatus:
    NOT_READY = 0
    READY = 1
    DONE = 2


class NavigationSystem:
    def __init__(self):
        self.status = NavigationStatus.NOT_READY
    

    def get_status(self) -> NavigationStatus:
        return self.status


    def set_status(self, new_status: NavigationStatus) -> None:
        self.status = new_status


    def Serial_Teensy(self):
        ''' TODO: This method needs to be renamed.
            This method starts a serial listener that waits
            for the Teensy messages. It will be used to know when
            to start the collection mechanism, when to stop it, and
            any other Teensy-Pi communication tasks'''
        pass
    