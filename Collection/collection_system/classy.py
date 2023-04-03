import cv2
import asyncio
from time import sleep
from FlipperPlatform import FlipperPlatform
from Robot import Robot, RobotStatus, CollectionStatus
from Drum import DrumStatus
from BraceCode import BraceStatus
from CameraSystem import CameraSystem
from IntakeSystem import IntakeSystem

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
    flipper = robot.CollectionSystem.Flipper
    state = robot.CollectionSystem.status

    try:
        with camera.camera as cam:
            
            while True:
                elevator_data, mid_data, flipper_data = camera.get_data()

                if cv2.waitKey(1) == 27:
                    break
                

                if state == CollectionStatus.INTAKE:
                    if not flipper_data:
                        robot.CollectionSystem.Intake.StartIntake()
                    else:
                        robot.CollectionSystem.Intake.StopIntake()
                        state = CollectionStatus.FLIPPER_ORIENT



                elif state == CollectionStatus.FLIPPER:
                    pass
                    
                    ''' --------------------------------- '''
                while len(flipper_data) < 1:
                    _, _, flipper_data = camera.get_data()
                    print(flipper_data)

                
                robot.CollectionSystem.Intake.StopIntake()


                #0 Class Ex. Yellow Duck, column
                #1 Bounding Box Ex. Not necessary
                #2 Angle Ex. Degrees, we want lower that 1
                #TODO: Make sure you include clearing code for overflow items such as ducks
                #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                #Recieves a valid input from the intake, then the flipper code begins
                ItemToSort = flipper_data[0][0]
                AngleToTurn = flipper_data[0][2]
                while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                    flipper.rotate_platform()
                    while len(flipper_data) < 1:
                        _, _, flipper_data = camera.get_data()
                        AngleToTurn = flipper_data[0][2]
                    
                    flipper.set_current_angle(AngleToTurn)
                        


                flipper.stop_rotation()
                while True:
                    _, _, flipper_data = camera.get_data()
                asyncio.run(flipper.flip_platform())



                robot.CollectionSystem.Flipper.StopRotation()
                robot.CollectionSystem.Flipper.FlipPlatform()
                #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                
                #From here we do all that is necessary with the elevator
                #sleep(2)
                elevator_data = []
                while len(elevator_data) < 1:
                    elevator_data, _, _ = camera.get_data()
                    AngleToTurn = elevator_data[0][2]
                
                while AngleToTurn > Global_Static.PILLAR_MAX_THRESH or AngleToTurn < Global_Static.PILLAR_MIN_THRESH:
                    robot.CollectionSystem.Elevator.RotatePlatform()
                    sleep(2)
                    while len(elevator_data) < 1:
                        elevator_data, _, _ = camera.get_data()
                        AngleToTurn = elevator_data[0][2]
                
                #The object is now oriented and ready to be raised
                robot.CollectionSystem.Elevator.raisePlatformToColumn()
                #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                
                robot.CollectionSystem.Pushers.LoadingPillarPusher1()
                
                robot.CollectionSystem.Drum.stateMachineInput(DrumStatus.SLOT1OUT)
            
            cv2.destroyAllWindows()
                
            
            
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()


if __name__ == '__main__':
  main()
