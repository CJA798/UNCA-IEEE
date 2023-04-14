import BraceSystem
import Pusher






def TowerSequenceFinal1(serial_stepper, robot: Robot):
        #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower.
    PusherPosition = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns



    #start with white column, assuming it is loaded
    PusherPosition = outputSerial(serial_stepper, 'C') #white column
    if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
        robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace


    if columnPosition[0] == 3:
        PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace

    else:
        PusherPosition = outputSerial(serial_stepper, 'D')
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
    
    PusherPosition = outputSerial(serial_stepper, 'A')
    robot.CollectionSystem.BraceSystem.twoTowerset()
    robot.CollectionSystem.Pushers.UnloadingPillarPusherBot()
    robot.CollectionSystem.BraceSystem.twoTowerDrop()
    robot.CollectionSystem.BraceSystem.twoTowerClose()

def TowerSequenceFinal2(serial_stepper, robot: Robot):
        #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower.
    PusherPosition = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns



    #start with white column, assuming it is loaded
    PusherPosition = outputSerial(serial_stepper, 'C') #white column
    if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #full unloading top pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace


    if columnPosition[0] == 3:
        PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace

    else:
        PusherPosition = outputSerial(serial_stepper, 'D')
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
    
    PusherPosition = outputSerial(serial_stepper, 'A')
    robot.CollectionSystem.BraceSystem.twoTowerset()
    robot.CollectionSystem.Pushers.UnloadingPillarPusherBot()
    robot.CollectionSystem.BraceSystem.twoTowerDrop()
    robot.CollectionSystem.BraceSystem.twoTowerClose()

def TowerSequenceFinal3(serial_stepper, robot: Robot):
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
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            greenColumnCounter = greenColumnCounter - 1
        
        if greenColumnCounter == 2 and len(columnPosition) > 1:
            #Red/Green column position, this case is for when red is first
            PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.pop()
        
        if greenColumnCounter == 2 and columnPosition == 1:
            #Pink Duck position
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #If the tower isn't build yet then tell the while loop to continue
    return 0
            










    

'''
if columnPosition[0] == 3:
        #initialize brace
        robot.CollectionSystem.BraceSystem.BraceActive() #Loading and shaking brace
        #perfect case.
        if len(columnPosition) > 0:
            PusherPosition = outputSerial(serial_stepper, 'C') #white column
        if PusherPosition == PusherStatus.READY and len(columnPosition) > 0:
            #robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
            
        
        if len(columnPosition) == 0:
            PusherPosition = outputSerial(serial_stepper, 'D')
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
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            greenColumnCounter = greenColumnCounter - 1
        
        if greenColumnCounter == 2 and len(columnPosition) > 1:
            #Red/Green column position, this case is for when red is first
            PusherPosition = outputSerial(serial_stepper, 'E')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            columnPosition.pop()
        
        if greenColumnCounter == 2 and columnPosition == 1:
            #Pink Duck position
            PusherPosition = outputSerial(serial_stepper, 'B')
        if PusherPosition == PusherStatus.READY and greenColumnCounter == 2 and len(columnPosition) > 1:
            #Once this if statement finishes then we tell waiting to finish that we are done and need to wait for the next tower to be ready to be built
            robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
            robot.CollectionSystem.Pushers.RetractPusherTop()
            PusherPosition = PusherStatus.RETRACTED
            return 1
    #If the tower isn't build yet then tell the while loop to continue
    return 0
'''


    