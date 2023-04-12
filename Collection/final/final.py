import cv2
import asyncio
import serial
from time import sleep
from FlipperPlatform import Flipper, FlipperStatus
from Robot import Robot, RobotStatus
from BraceSystem import BraceStatus
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
    Y_D_ANG_MIN_THRESH = 10
    Y_D_ANG_MAX_THRESH = 40
    #Pink Duck angle threshhold
    P_D_ANG_MIN_THRESH = 70
    P_D_ANG_MAX_THRESH = 100
    #All pillar threshholds
    PILLAR_MIN_THRESH = 10
    PILLAR_MAX_THRESH = 30
DrumStatus = 0
yellowDuckCounter = 0
columnPosition = []
greenColumnCounter = 0
whiteColumnCounter = 0
TOWER_ONE = 'a'
TOWER_TWO = 'b'
DUCK_TOWER = 'c'

        


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
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.clear
        
        if len(columnPosition) == 0:
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and len(columnPosition) == 0:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The red/green columns are backwards so I need to unload a green column from the other slot then the red. Finally the pink
    elif columnPosition[0] == 4:
        if greenColumnCounter > 2:
            #Green column position
            PusherPosition = outputSerial(serial_stepper, 'D')
        if PusherPosition == PusherStatus.READY and greenColumnCounter > 2:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop()
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            greenColumnCounter = greenColumnCounter - 1
        
        if greenColumnCounter == 2 and len(columnPosition) > 1:
            #Red/Green column position, this case is for when red is first
            PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop()
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.pop()
        
        if greenColumnCounter == 2 and columnPosition == 1:
            #Pink Duck position
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            sleep(.50)
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #If the tower isn't build yet then tell the while loop to continue
    return 0
            
        
#Serial Communication for the drum if you only need to write a known position. There is another for intake and finding the appropriate slot
def outputSerial(serial_stepper, position):
    global DrumStatus
    if DrumStatus == 0:
        serial_stepper.reset_output_buffer()
        str_data = "Input: " + position + '\n'
        serial_stepper.write(str_data.encode())
        DrumStatus = 1
    elif DrumStatus == 1:
        if serial_stepper.in_waiting > 0:
            DrumStatus = serial_stepper.readline().decode('utf-8').rstrip()
            serial_stepper.reset_input_buffer()
    elif DrumStatus == "finished" + position:
        DrumStatus = 0
        return PusherStatus.READY


  
CurrItem = -1
def CollectionStateMachine(robot: Robot, elevator_data, mid_data, flipper_data, serial_stepper, camera: CameraSystem):
    #this is the state machine for the entire system.
    global CurrItem
    global DrumStatus
    global yellowDuckCounter
    global columnPosition
    global greenColumnCounter
    global whiteColumnCounter

    flipper_status = robot.CollectionSystem.Flipper.status
    elevator_status = robot.CollectionSystem.Elevator.status
    pusher_status = robot.CollectionSystem.Pushers.statusBot
    intake_status = robot.CollectionSystem.Intake.status
    if len(elevator_data) > 0:
        CurrItem = elevator_data[0][0]
    #I am going to have to make a state machine for each of the objects.
    #I will start from the beginning and move to the end. Should be easy...
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #The intake states that can happen and what will be happening.
    '''if intake_status == IntakeStatus.INTAKE_OFF and len(flipper_data) < 1:
        robot.CollectionSystem.Intake.StartIntake()
        robot.CollectionSystem.Intake.status = IntakeStatus.INTAKE_ON
    elif len(flipper_data) > 0:
        robot.CollectionSystem.Intake.StopIntake()
        robot.CollectionSystem.Intake.status = IntakeStatus.INTAKE_OFF
    elif False:
        pass #We are just going to make a timing function for the intake if there is some kind of jam.
    '''
    '''Check how many ducks we have when this item is brought into the system. This function may need to be moved around to push out or in depending on the elevator'''
    if len(flipper_data) > 0 and elevator_status == ElevatorStatus.READY:
        if len(flipper_data) > 1:
            robot.CollectionSystem.Sweep.sweep()
            print("Too many Items")
            print(len(flipper_data))
            print(flipper_data)
        if flipper_data[0][0] == 0 and yellowDuckCounter == 2:
            robot.CollectionSystem.Sweep.sweep()
            print("Too many Ducks")
    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Flipper statuses and what it will be doing. The flipped case is ignored since it will need to consider the item for each case of flip or sweep
    if len(flipper_data) > 0 and flipper_status != FlipperStatus.ORIENTED:
        #Here I have to consider all of the ducks and columns
        if flipper_data[0][0] <= 1:
            if flipper_data[0][2] > Global_Static.Y_D_ANG_MAX_THRESH or flipper_data[0][2] < Global_Static.Y_D_ANG_MIN_THRESH:
                #Orienting the duck
                robot.CollectionSystem.Flipper.rotate_platform()
            else:
                #Duck is oriented
                robot.CollectionSystem.Flipper.stop_rotation()
                flipper_status = robot.CollectionSystem.Flipper.status = FlipperStatus.ORIENTED
        elif flipper_data[0][0] >= 2:
            if flipper_data[0][2] > Global_Static.PILLAR_MAX_THRESH or flipper_data[0][2] < Global_Static.PILLAR_MIN_THRESH:
                #Orienting the Column
                robot.CollectionSystem.Flipper.rotate_platform()
                print(flipper_data[0][2])
            else:
                #Oriented
                robot.CollectionSystem.Flipper.stop_rotation()
                flipper_status = robot.CollectionSystem.Flipper.status = FlipperStatus.ORIENTED
                print("job done")
    
    '''
    This area is for Carlos to determine whether the object needs to be sweeped or flipped into the elevator. 
    
    
    '''
    if len(flipper_data) > 0 and flipper_status == FlipperStatus.ORIENTED and elevator_status == ElevatorStatus.READY:
        duck_standing = camera.is_duck_standing3()
        if duck_standing:
            robot.CollectionSystem.Sweep.push()
            CurrItem = flipper_data[0][0]
        else:
            robot.CollectionSystem.Flipper.flip_platform()
            CurrItem = flipper_data[0][0]
            
        elevator_status = robot.CollectionSystem.Elevator.status = ElevatorStatus.FILLED
        flipper_status = robot.CollectionSystem.Flipper.status = FlipperStatus.EMPTY
        return
    
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #elevator states
    if len(elevator_data) > 0 and elevator_status == ElevatorStatus.FILLED:
        #Here I need to do a few things. First is consider whether or not the object is oriented.
        #After that then I need to move the elevator up to the desired height and let the pushers know that I'm ready and to wait for drum response.
        if elevator_data[0][0] <= 1:
            if elevator_data[0][2] > Global_Static.Y_D_ANG_MAX_THRESH or elevator_data[0][2] < Global_Static.Y_D_ANG_MIN_THRESH:
                robot.CollectionSystem.Elevator.rotate_platform()
            else:
                robot.CollectionSystem.Elevator.stop_rotation()
                elevator_status = robot.CollectionSystem.Elevator.status = ElevatorStatus.ORIENTED_OBJECT
        elif elevator_data[0][0] >= 2:
            if elevator_data[0][2] > Global_Static.PILLAR_MAX_THRESH or elevator_data[0][2] < Global_Static.PILLAR_MIN_THRESH:
                robot.CollectionSystem.Elevator.rotate_platform()
            else:
                robot.CollectionSystem.Elevator.stop_rotation()
                elevator_status = robot.CollectionSystem.Elevator.status = ElevatorStatus.ORIENTED_OBJECT
    
    #Once the elevator is oriented then it will raise the object
    
    if elevator_status == ElevatorStatus.ORIENTED_OBJECT:
        '''Elevator needs to be updated to raise to a single position instead of having a duck raise and column raise'''
        if elevator_data[0][0] == 0:
            yellowDuckCounter = yellowDuckCounter + 1
        elif elevator_data[0][0] == 3:
            greenColumnCounter = greenColumnCounter + 1
        elif elevator_data[0][0] == 2:
            whiteColumnCounter = whiteColumnCounter + 1
        robot.CollectionSystem.Elevator.raisePlatform()
          
    elif pusher_status != PusherStatus.READY and CurrItem >= 0 and CurrItem <= 4:
        pusher_status = robot.CollectionSystem.Pushers.statusBot = drumSerial(CurrItem, serial_stepper, robot)
        
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #This is pusher states. Once the pusher is done then it will also set the elevator status to ready state.
    
    if pusher_status == PusherStatus.READY:
        '''This still needs to have all of the pusher class updated. We will also need the elevator class to be updated.
        The pusher class needs to have two separate pushers. One for top and one for bottom. Those have their specified 
        functions that will move them independently'''
        robot.CollectionSystem.Pushers.LoadingPillarPusher()
        robot.CollectionSystem.Elevator.lowerToGround()
        CurrItem = -1
        
        
        
TimeToUnload = 0
def firstWhiteCol(position, serial_stepper, robot: Robot):
    global TimeToUnload
    global CurrItem

    if TimeToUnload == 0:
        pusherStatus = outputSerial(serial_stepper, position)
        if pusherStatus == PusherStatus.READY:
            robot.CollectionSystem.Pushers.LoadingPillarPusher()
            TimeToUnload = 1

    if TimeToUnload == 1:
        pusherStatus = outputSerial(serial_stepper, 'C')
        if pusherStatus == PusherStatus.READY:
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            robot.CollectionSystem.Elevator.lowerToGround()
            CurrItem = -1

position = ''
def drumSerial(item, serial_stepper, robot):
    global DrumStatus
    global position
    
    if DrumStatus == 0:
        position = positionSelection(item, serial_stepper, robot)
        if position == 'z':
            return
    if position != None:
        if DrumStatus == 0 and position != '':
            serial_stepper.reset_output_buffer()
            str_data = "Input: " + position + "\n"
            serial_stepper.write(str_data.encode())
            DrumStatus = 1
        elif DrumStatus == 1:
            if serial_stepper.in_waiting > 0:
                DrumStatus = serial_stepper.readline().decode('utf-8').rstrip()
                print("Read the line")
            if DrumStatus != "finished" + position:
                DrumStatus = 1
        elif DrumStatus == "finished" + position:
            DrumStatus = 0
            position = ''
            return PusherStatus.READY
    
def positionSelection(item, serial_stepper, robot: Robot):
    global DrumStatus
    global yellowDuckCounter
    global columnPosition
    global greenColumnCounter
    global whiteColumnCounter
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Yellow Duck case of the drum
    if item == 0 and yellowDuckCounter == 1:
        return 'a'
    elif item == 0 and yellowDuckCounter == 2:
        return 'f'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Pink duck loading
    elif item == 1:
        return 'b'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #White Column positioning. We need to worry about if this is the first time or not.
    #If it is the first time then we need to load then unload immediately.
    #Any time after that we just need to load
    elif item == 2 and whiteColumnCounter == 1:
        #We need to worry about loading then unloading. After this we can move onto the next object.
        firstWhiteCol('c', serial_stepper, robot)
        return 'z'
        
    elif item == 2 and whiteColumnCounter > 1:
        return 'c'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Green column case. If this is the first two columns then we will just load into the green slot
    #If this is the third case then we need to load it in the red/green slot and save that location for later use
    elif item == 3 and greenColumnCounter <= 2:
        return 'd'
    elif item == 3 and greenColumnCounter == 3:
        columnPosition.append(3)
        return 'e'
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #This is loading the red column and saving the position
    elif item == 4:
        columnPosition.append(4)
        return 'e'
        

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
                #if not elevator_data and not mid_data and not flipper_data:
                 #   continue
                if cv2.waitKey(1) == 27:
                    break
                #I think all cases of the intake has been completed
                CollectionStateMachine(robot, elevator_data, mid_data, flipper_data, serial_stepper, camera)
                
                '''We are going to have to figure out something right here to solve'''
                '''if serial_stepper.in_waiting > 0:
                    DrumStatus = serial_stepper.readline().decode('utf-8').rstrip()
                    serial_stepper.reset_input_buffer()
                    
                if DrumStatus == "Output" and len(elevator_data) < 1 and len(flipper_data) < 1:
                    break'''
                

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


if __name__ == '__main__':
  main()