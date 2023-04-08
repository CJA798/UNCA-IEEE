import cv2
import asyncio
import serial
from time import sleep
from FlipperPlatform import FlipperPlatform, FlipperStatus
from Robot import Robot, RobotStatus, CollectionStatus
from Brace import BraceStatus
from CameraSystem import CameraSystem
from IntakeSystem import IntakeSystem, IntakeStatus
from PusherSystem import PusherStatus
from ElevatorPlatform import ElevatorStatus

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
DrumStatus = 0
def main():
    # Create objects
    camera = CameraSystem()
    robot = Robot()
    DrumStatus = 0
    
    baud_rate = 115200
    serial_stepper = serial.Serial('/dev/ttyACM0', baud_rate)
    serial_stepper.reset_input_buffer()

    try:
        with camera.camera as cam:
            
            while True:
                elevator_data, mid_data, flipper_data = camera.get_data()

                if cv2.waitKey(1) == 27:
                    break
                
                CollectionStateMachine(robot, elevator_data, mid_data, flipper_data, serial_stepper)
                

            cv2.destroyAllWindows()
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()

def ElevatorOrientLow(elevator_data, AngleToTurn, robot, camera):
    robot.CollectionSystem.Elevator.RotatePlatform()
    robot.CollectionSystem.Elevator.StopRotation()

if __name__ == '__main__':
  main()
  
  
def CollectionStateMachine(robot: Robot, elevator_data, mid_data, flipper_data, serial):
    flipper_status = robot.CollectionSystem.Flipper.status
    elevator_status = robot.CollectionSystem.Elevator.status
    #pusherBot_status = robot.CollectionSystem.Pushers.statusBot
    #pusherTop_status = robot.CollectionSystem.Pushers.statusTop
    brace_status = robot.CollectionSystem.Brace.status
    intake_status = robot.CollectionSystem.Intake.status
    currentObject = elevator_data[0][0]
    #I am going to have to make a state machine for each of the objects.
    #I will start from the beginning and move to the end. Should be easy...
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The intake states that can happen and what will be happening.
    if intake_status == IntakeStatus.INTAKE_OFF and len(flipper_data) < 1:
        robot.CollectionSystem.Intake.StartIntake()
        robot.CollectionSystem.Intake.status = IntakeStatus.INTAKE_ON
    elif len(flipper_data) > 1:
        robot.CollectionSystem.Intake.StopIntake
        robot.CollectionSystem.Intake.status = IntakeStatus.INTAKE_OFF
    elif False:
        pass #We are just going to make a timing function for the intake if there is some kind of jam.
    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Flipper statuses and what it will be doing. The flipped case is ignored since it will need to consider the item for each case of flip or sweep
    if len(flipper_data) > 0 and flipper_status != FlipperStatus.ORIENTED:
        #Here I have to consider all of the ducks and columns
        if flipper_data[0][0] <= 1:
            if flipper_data[0][2] > Global_Static.Y_D_ANG_MAX_THRESH or flipper_data[0][2] < Global_Static.Y_D_ANG_MIN_THRESH:
                #Orienting the duck
                robot.CollectionSystem.Flipper.rotate_platform
            else:
                #Duck is oriented
                robot.CollectionSystem.Flipper.stop_rotation
                flipper_status = FlipperStatus.ORIENTED
        elif flipper_data[0][0] >= 2:
            if flipper_data[0][2] > Global_Static.Y_D_ANG_MAX_THRESH or flipper_data[0][2] < Global_Static.Y_D_ANG_MIN_THRESH:
                #Orienting the Column
                robot.CollectionSystem.Flipper.rotate_platform
            else:
                #Oriented
                robot.CollectionSystem.Flipper.stop_rotation
                flipper_status = FlipperStatus.ORIENTED
    
    '''
    This area is for Carlos to determine whether the object needs to be sweeped or flipped into the elevator. 
    
    
    '''
    #moveObject(robot, flipper_status)
    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #elevator states
    if len(elevator_data) > 0:
        #Here I need to do a few things. First is consider whether or not the object is oriented.
        #After that then I need to move the elevator up to the desired height and let the pushers know that I'm ready and to wait for drum response.
        if elevator_data[0][0] <= 1:
            if flipper_data[0][2] > Global_Static.Y_D_ANG_MAX_THRESH or elevator_data[0][2] < Global_Static.Y_D_ANG_MIN_THRESH:
                robot.CollectionSystem.Elevator.RotatePlatform
            else:
                robot.CollectionSystem.Elevator.StopRotation
                elevator_status = ElevatorStatus.ORIENTED_OBJECT
        elif elevator_data[0][0] >= 2:
            if elevator_data[0][2] > Global_Static.PILLAR_MAX_THRESH or elevator_data[0][2] < Global_Static.PILLAR_MIN_THRESH:
                robot.CollectionSystem.Elevator.RotatePlatform
            else:
                robot.CollectionSystem.Elevator.StopRotation
                elevator_status = ElevatorStatus.ORIENTED_OBJECT
    
    #Once the elevator is oriented then it will raise the object
    
    if elevator_status == ElevatorStatus.ORIENTED_OBJECT:
        '''Elevator needs to be updated to raise to a single position instead of having a duck raise and column raise'''
        robot.CollectionSystem.Elevator.raisePlatformToDuck
        sleep(.25)
        elevator_status = ElevatorStatus.RAISED
    elif elevator_status == ElevatorStatus.RAISED and pusher_status != PusherStatus.READY:
        pusher_status = drumSerial(elevator_data[0][0], serial)
        
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #This is pusher states. Once the pusher is done then it will also set the elevator status to ready state.
    
    if pusher_status == PusherStatus.READY:
        '''This still needs to have all of the pusher class updated. We will also need the elevator class to be updated.
        The pusher class needs to have two separate pushers. One for top and one for bottom. Those have their specified 
        functions that will move them independently'''
        robot.CollectionSystem.Pushers.LoadingPillarPusher1
        sleep(.25)
        robot.CollectionSystem.Pushers.RetractPusher
        sleep(.25)
        robot.CollectionSystem.Elevator.lowerToGround
        sleep(.25)
        elevator_status = ElevatorStatus.READY
        pusher_status = PusherStatus.RETRACTED
    
    
def drumSerial(item, serial, pusher_status):
    if DrumStatus == 0:
        serial.reset_output_buffer()
        str_data = str(int_data) + "\n"
        serial.write(str_data.encode())
        DrumStatus = 1
    elif DrumStatus == 1:
        DrumStatus = serial.readline().decode('utf-8').rstrip()
    elif DrumStatus == "Something":
        DrumStatus = 0
        return PusherStatus.READY