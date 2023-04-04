import cv2
import asyncio
from time import sleep
from ElevatorPlatform import Elevator, ElevatorStatus
from FlipperPlatform import FlipperPlatform, FlipperStatus
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
    elevator = robot.CollectionSystem.Elevator

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
                        state = CollectionStatus.FLIPPER


                elif state == CollectionStatus.FLIPPER:
                    if not flipper_data or len(flipper_data) < 3:
                        flipper.set_status(FlipperStatus.EMPTY)
                        print("not-flipper_data")
                    else:
                        flipper.set_current_angle(flipper_data[0][3])
                        print(flipper.get_current_angle())
                        if flipper.get_current_angle() > 95 or flipper.get_current_angle() < 80:
                            flipper.rotate_platform()
                            print("Orienting")
                        else:
                            flipper.stop_rotation()
                            asyncio.run(flipper.flip_platform())
                            state = CollectionStatus.ELEVATOR_ROTATION


                
                elif state == CollectionStatus.ELEVATOR_ROTATION:
                    if not elevator_data or len(elevator_data) < 3:
                        elevator.set_status(ElevatorStatus.EMPTY)
                    else:
                        elevator.set_current_angle(elevator_data[0][3])
                        if elevator.getAngle() > 95 or elevator.getAngle() < 80:
                            elevator.rotate_platform()
                        else:
                            elevator.stop_rotation()
                            asyncio.run(elevator.flip_platform())
                            state = CollectionStatus.ELEVATOR_UPDOWN

                
                elif state == CollectionStatus.ELEVATOR_UPDOWN:
                    if not elevator_data or len(elevator_data) < 3:
                        elevator.set_status(ElevatorStatus.EMPTY)
                    else:

                        asyncio.run(elevator.elevate_platform)
                        height = elevator.hat.get_servo_position()
                        if height == 0:
                            elevator.rotate_platform()
                        else:
                            elevator.stop_rotation()
                            asyncio.run(elevator.flip_platform())
                            state = CollectionStatus.PUSHER


                elif state == CollectionStatus.PUSHER:
                    pass

                else:
                    continue

                print(state)
            cv2.destroyAllWindows()
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()


if __name__ == '__main__':
  main()
