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
yellowDuckCounter = 0
columnPosition = []
greenColumnCounter = 0
whiteColumnCounter = 0
TOWER_ONE = 'a'
TOWER_TWO = 'b'
DUCK_TOWER = 'c'

def main():
    # Create objects
    camera = CameraSystem()
    robot = Robot()
    
    
    baud_rate = 115200
    serial_stepper = serial.Serial('/dev/ttyACM0', baud_rate)
    serial_stepper.reset_input_buffer()

    try:
        with camera.camera as cam:
            
            while True:
                elevator_data, mid_data, flipper_data = camera.get_data()

                if cv2.waitKey(1) == 27:
                    break
                #I think all cases of the intake has been completed
                CollectionStateMachine(robot, elevator_data, mid_data, flipper_data, serial_stepper)
                if serial_stepper.in_waiting > 0:
                    DrumStatus = serial.readline().decode('utf-8').rstrip()
                    
                if DrumStatus == "Output" and len(elevator_data) < 1 and len(flipper_data) < 1:
                    break
                

            cv2.destroyAllWindows()
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()
        
    DrumStatus = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #This is the outpt function, It should be hard coded depending on the serial from the navigation Teensy
    while True:
        currentTower = ''
        if serial_stepper.in_waiting > 0:
            currentTower = serial.readline().decode('utf-8').rstrip()
        
        if currentTower != '':
            WaitingToFinish = 0
            
            while not WaitingToFinish:
                if currentTower == TOWER_ONE:
                    WaitingToFinish = sequenceForTower1(serial_stepper, robot)
                if currentTower == TOWER_TWO:
                    WaitingToFinish = sequenceForTower2(serial_stepper, robot)
                if currentTower == DUCK_TOWER:
                    WaitingToFinish = sequenceForTower3(serial_stepper, robot)
        


def sequenceForTower1(serial_stepper, robot: Robot):
    pass

def sequenceForTower2(serial_stepper, robot: Robot):
    pass

def sequenceForTower3(serial_stepper, robot: Robot):
    #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower.
    PusherPosition = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns
    if columnPosition[0] == 3:
        #perfect case.
        if len(columnPosition) > 0:
            PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.clear
        
        if len(columnPosition) == 0:
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and len(columnPosition) == 0:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The red/green columns are backwards so I need to unload a green column from the other slot then the red. Finally the pink
    elif columnPosition[0] == 4:
        if greenColumnCounter > 2:
            PusherPosition = outputSerial(serial_stepper, 'D')
        if PusherPosition == PusherStatus.READY and greenColumnCounter > 2:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop
            PusherPosition = PusherStatus.RETRACTED
            greenColumnCounter = greenColumnCounter - 1
        
        if greenColumnCounter == 2 and len(columnPosition) > 1:
            PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.pop
        
        if greenColumnCounter == 2 and columnPosition == 1:
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #If the tower isn't build yet then tell the while loop to continue
    return 0
            
        
    pass

def outputSerial(serial_stepper, position):
    if DrumStatus == 0:
        serial.reset_output_buffer()
        str_data = position
        serial.write(str_data.encode())
        DrumStatus = 1
    elif DrumStatus == 1:
        if serial_stepper.in_waiting > 0:
            DrumStatus = serial.readline().decode('utf-8').rstrip()
    elif DrumStatus == 'R':
        DrumStatus = 0
        return PusherStatus.READY

if __name__ == '__main__':
  main()
  
  
def CollectionStateMachine(robot: Robot, elevator_data, mid_data, flipper_data, serial_stepper):
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
    
    '''Check how many ducks we have when this item is brought into the system. This function may need to be moved around to push out or in depending on the elevator'''
    
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
        pusher_status = drumSerial(elevator_data[0][0], serial_stepper)
        if elevator_data[0][0] == 0:
            yellowDuckCounter = yellowDuckCounter + 1
        if elevator_data[0][0] == 3:
            greenColumnCounter = greenColumnCounter + 1
        if elevator_data[0][0] == 2:
            whiteColumnCounter = whiteColumnCounter + 1
                    
                
        
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
        
    
    
def drumSerial(item, serial_stepper):
    position = ''
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Yellow Duck case of the drum
    if item == 0 and yellowDuckCounter == 1:
        position = 'a'
    elif item == 0 and yellowDuckCounter == 2:
        position = 'f'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Pink duck loading
    if item == 1:
        position = 'b'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #White Column positioning. We need to worry about if this is the first time or not.
    #If it is the first time then we need to load then unload immediately.
    #Any time after that we just need to load
    if item == 2 and whiteColumnCounter == 1:
        #We need to worry about loading then unloading. After this we can move onto the next object.
        positon = 'c'
        firstWhiteCol(position, serial_stepper)
        position = ''
        return
    elif item == 2 and whiteColumnCounter > 1:
        position = 'c'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Green column case. If this is the first two columns then we will just load into the green slot
    #If this is the third case then we need to load it in the red/green slot and save that location for later use
    if item == 3 and greenColumnCounter <= 2:
        position = 'd'
    elif item == 3 and greenColumnCounter == 3:
        columnPosition.append(3)
        position = 'e'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #This is loading the red column and saving the position
    if item == 4:
        columnPosition.append(4)
        position = 'e'
    
    if DrumStatus == 0:
        serial_stepper.reset_output_buffer()
        str_data = position
        serial_stepper.write(str_data.encode())
        DrumStatus = 1
    elif DrumStatus == 1:
        if serial_stepper.in_waiting > 0:
            DrumStatus = serial.readline().decode('utf-8').rstrip()
    elif DrumStatus == "Something":
        DrumStatus = 0
        return PusherStatus.READY