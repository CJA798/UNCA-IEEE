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
    PILLAR_MIN_THRESH = 110
    PILLAR_MAX_THRESH = 150

def main():
    # Create objects
    camera = CameraSystem()
    robot = Robot()
    

    try:
        while True:
            elevator_data, middle_data, flipper_data = await camera.get_data()
            if cv2.waitKey(1) == 27:
                break
            '''
            Intake code here for everything to really get started.
            '''
            #0 Class Ex. Yellow Duck, column
            #1 Bounding Box Ex. Not necessary
            #2 Angle Ex. Degrees, we want lower that 15
            if len(flipper_data) > 1:
                robot.CollectionSystem.Flipper.clear_platform
                continue
            #TODO: Make sure you include clearing code for overflow items such as ducks
            
            
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #Recieves a valid input from the intake, then the flipper code begins
            ItemToSort = flipper_data[0][0]
            AngleToTurn = flipper_data[0][2]
            if ItemToSort == Global_Static.YELLOW_DUCK:
                while AngleToTurn > Global_Static.Y_D_ANG_MAX_THRESH or AngleToTurn < Global_Static.Y_D_ANG_MIN_THRESH:
                    robot.CollectionSystem.Flipper.rotate_platform
                    robot.CollectionSystem.Flipper.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            elif ItemToSort == Global_Static.PINK_DUCK:
                while AngleToTurn > Global_Static.P_D_ANG_MAX_THRESH or AngleToTurn < Global_Static.P_D_ANG_MIN_THRESH:
                    robot.CollectionSystem.Flipper.rotate_platform
                    robot.CollectionSystem.Flipper.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            elif ItemToSort == Global_Static.GREEN_PILLAR or ItemToSort == Global_Static.RED_PILLAR or ItemToSort == Global_Static.WHITE_PILLAR:
                while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                    robot.CollectionSystem.Flipper.rotate_platform
                    robot.CollectionSystem.Flipper.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            robot.CollectionSystem.Flipper.flip_platform
            #Flipper has oriented the object and is moving it onto the Elevator
            #Assuming that the flip is correct with no errors at the moment
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #Starting the code to Orient the object
            AngleToTurn = flipper_data[0][2]
            if ItemToSort == Global_Static.YELLOW_DUCK:
                while AngleToTurn > Global_Static.Y_D_ANG_MAX_THRESH or AngleToTurn < Global_Static.Y_D_ANG_MIN_THRESH:
                    robot.CollectionSystem.Elevator.rotate_platform
                    robot.CollectionSystem.Elevator.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            elif ItemToSort == Global_Static.PINK_DUCK:
                while AngleToTurn > Global_Static.P_D_ANG_MAX_THRESH or AngleToTurn < Global_Static.P_D_ANG_MIN_THRESH:
                    robot.CollectionSystem.Elevator.rotate_platform
                    robot.CollectionSystem.Elevator.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            elif ItemToSort == Global_Static.GREEN_PILLAR or ItemToSort == Global_Static.RED_PILLAR or ItemToSort == Global_Static.WHITE_PILLAR:
                while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                    robot.CollectionSystem.Elevator.rotate_platform
                    robot.CollectionSystem.Elevator.stop_rotation
                    AngleToTurn = flipper_data[0][2]
            
            #Raising the platform to the correct height to prepare to be pushed
            if ItemToSort == Global_Static.YELLOW_DUCK or ItemToSort == Global_Static.PINK_DUCK:
                robot.CollectionSystem.Elevator.raisePlatformToDuck
            elif ItemToSort == Global_Static.GREEN_PILLAR or ItemToSort == Global_Static.RED_PILLAR or ItemToSort == Global_Static.WHITE_PILLAR:
                robot.CollectionSystem.Elevator.raisePlatformToColumn
            #Elevator is done, it is at the correct level and oriented.
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #Going to rotate the drum to prepare for item to be pushed
            match ItemToSort:
                case Global_Static.YELLOW_DUCK:
                    if robot.CollectionSystem.Drum.yellowDuckSlots[0] == 0:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.YELLOW_DUCK1)
                    elif robot.CollectionSystem.Drum.yellowDuckSlots[1] == 0:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.YELLOW_DUCK2)
                    
                case Global_Static.PINK_DUCK:
                    robot.CollectionSystem.Drum.State_Machine(DrumStatus.PINK_DUCK)
                    
                case Global_Static.RED_PILLAR:
                    robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR1)
                    if robot.CollectionSystem.Drum.pillarSlot1.pillarState[0] != 0:
                        robot.CollectionSystem.Drum.pillarSlot1.pillarState[0] = 1
                        robot.CollectionSystem.Drum.pillarSlot1.pillarColor[0] = "Red"
                    else:
                        robot.CollectionSystem.Drum.pillarSlot1.pillarState[1] = 1
                        robot.CollectionSystem.Drum.pillarSlot1.pillarColor[1] = "Red"
                    
                case Global_Static.GREEN_PILLAR:
                    #Puts the columns in the 2 spot first
                    if robot.CollectionSystem.Drum.pillarSlot2.pillarState[0] == 0:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR2)
                        robot.CollectionSystem.Drum.pillarSlot2.pillarState[0] = 1
                    #This will fill the last spot if it is open
                    elif robot.CollectionSystem.Drum.pillarSlot2.pillarState[1] == 0:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR2)
                        robot.CollectionSystem.Drum.pillarSlot2.pillarState[1] = 1
                    #If we get 3 green columns in a row, put it in the first slot
                    elif robot.CollectionSystem.Drum.pillarSlot1.pillarState[0] == 0:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR1)
                        robot.CollectionSystem.Drum.pillarSlot1.pillarState[1] = 1
                    #If this is the last possible slot open
                    else:
                        robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR1)
                        robot.CollectionSystem.Drum.pillarSlot1.pillarState[1] = 1
                case Global_Static.WHITE_PILLAR:
                    robot.CollectionSystem.Drum.State_Machine(DrumStatus.PILLAR3)
            #Rotating the drum is done. Now we need to begin the bracing.
            #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            #We need to consider Pink Duck, but for now this will have yellow only
            #TODO: Write a case statement for the pink or yellow ducks coming out of the bot
            #We are ignoring the pendulum for now and anything about a pink duck
            #robot.CollectionSystem.Brace.pendulum_activation
            if robot.CollectionSystem.Brace.status != BraceStatus.BRACE_BOTTOM_CLOSING:
                robot.CollectionSystem.Brace.brace_closed(7, BraceStatus.BRACE_BOTTOM_CLOSING)
            
                
            
            
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()


if __name__ == '__main__':
  main()
