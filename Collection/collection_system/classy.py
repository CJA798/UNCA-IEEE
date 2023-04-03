import cv2
import asyncio
from FlipperPlatform import FlipperPlatform, FlipperPlatformStatus
from Robot import Robot, RobotStatus
from Drum import DrumStatus
from BraceCode import BraceStatus
from CameraSystem import CameraSystem

class Global_Static:
    #This is how the camera is returning the values of the detection
    WHITE_PILLAR = 2
    RED_PILLAR = 4
    GREEN_PILLAR = 3
    YELLOW_DUCK = 0
    PINK_DUCK = 1
    #Yellow Duck Angle Threshhold
    Y_D_ANG_MIN_THRESH = 70
    Y_D_ANG_MAX_THRESH = 100
    #Pink Duck angle threshhold
    P_D_ANG_MIN_THRESH = 70
    P_D_ANG_MAX_THRESH = 100
    #All pillar threshholds
    PILLAR_MIN_THRESH = 50
    PILLAR_MAX_THRESH = 90

def main():
    # Create objects
    camera = CameraSystem()
    robot = Robot()
    

    try:
        while True:
            elevator_data, middle_data, flipper_data = camera.get_data()
            if cv2.waitKey(1) == 27:
                break
            '''
            Intake code here for everything to really get started.
            '''
            #0 Class Ex. Yellow Duck, column
            #1 Bounding Box Ex. Not necessary
            #2 Angle Ex. Degrees, we want lower that 1
            #TODO: Make sure you include clearing code for overflow items such as ducks
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #Recieves a valid input from the intake, then the flipper code begins
            ItemToSort = flipper_data[0][0]
            AngleToTurn = flipper_data[0][2]
            while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                robot.CollectionSystem.Flipper.rotate_platform(AngleToTurn)
                elevator_data, middle_data, flipper_data = camera.get_data()
                AngleToTurn = flipper_data[0][2]
            robot.CollectionSystem.Flipper.StopRotation
            robot.CollectionSystem.Flipper.FlipPlatform
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            
            #From here we do all that is necessary with the elevator
            elevator_data, middle_data, flipper_data = camera.get_data()
            AngleToTurn = flipper_data[0][2]
            
            while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                robot.CollectionSystem.Elevator.rotate_platform
                elevator_data, middle_data, flipper_data = camera.get_data()
                AngleToTurn = flipper_data[0][2]
            
            #The object is now oriented and ready to be raised
            robot.CollectionSystem.Elevator.raisePlatformToColumn
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            
            robot.CollectionSystem.Pushers.LoadingPillarPusher1
            
            robot.CollectionSystem.Drum.stateMachineInput(DrumStatus.SLOT1OUT)
            
            
                
            
            
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()


if __name__ == '__main__':
  main()
