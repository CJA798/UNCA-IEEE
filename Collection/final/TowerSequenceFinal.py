import BraceSystem
import PusherSystem







def TowerSequenceFinal1(serial_stepper, robot: Robot):
        #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower.
    TimeToUnload = 0
    PusherPosition = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns
    if TimeToUnload == 0 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, position)
    elif PusherPosition == PusherStatus.READY:
            robot.CollectionSystem.Pushers.LoadingPillarPusher()
            TimeToUnload = 1


    #start with white column, assuming it is loaded
    if TimeToUnload == 0 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'C') #white column
    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.Pushers.Half_UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
        TimeToUnload = 1


    if TimeToUnload == 1 and PusherPosition != PusherStatus.READY:
        if columnPosition[0] == 4:
            PusherPosition = outputSerial(serial_stepper, 'E')
        else:
            PusherPosition = outputSerial(serial_stepper, 'D')

    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
        TimeToUnload = 2

    if TimeToUnload == 2 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'A')
    elif PusherPosition == PusherStatus.READY:   
     robot.CollectionSystem.BraceSystem.twoTowerset()
     robot.CollectionSystem.Pushers.UnloadingPillarPusherBot()
     robot.CollectionSystem.BraceSystem.twoTowerDrop()
     robot.CollectionSystem.BraceSystem.twoTowerClose()
     #tell chase we are done 
     TimeToUnload = 0


def TowerSequenceFinal2(serial_stepper, robot: Robot):
        #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower.
    TimeToUnload = 0
    PusherPosition = 0
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns
    if TimeToUnload == 0 and PusherPosition != pusherStatus.READY:
        pusherStatus = outputSerial(serial_stepper, position)
    elif PusherPosition == pusherStatus.READY:
            robot.CollectionSystem.Pushers.LoadingPillarPusher()
            TimeToUnload = 1


    #start with white column, assuming it is loaded
    if TimeToUnload == 0 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'C') #white column
    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
        TimeToUnload = 1


    if TimeToUnload == 1 and PusherPosition != PusherStatus.READY:
        if columnPosition[0] == 4:
            PusherPosition = outputSerial(serial_stepper, 'E')
        else:
            PusherPosition = outputSerial(serial_stepper, 'D')

    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
        TimeToUnload = 2

    if TimeToUnload == 2 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'A')
    elif PusherPosition == PusherStatus.READY:   
     robot.CollectionSystem.BraceSystem.twoTowerset()
     robot.CollectionSystem.Pushers.UnloadingPillarPusherBot()
     robot.CollectionSystem.BraceSystem.twoTowerDrop()
     robot.CollectionSystem.BraceSystem.twoTowerClose()
     #tell chase we are done 
     TimeToUnload = 0

def TowerSequenceFinal3(serial_stepper, robot: Robot):
 #This will be the first case. Has to be, it will mean we can just make the tower
    #Cases to be considered. Do we have all the objects for this tower
    TimeToUnload = 0
    PusherPosition = 0

        #start with white column, assuming it is loaded
    if TimeToUnload == 0 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'C') #white column
    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop() #half unloading tp pusher 1 column inside the brace
        robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
        TimeToUnload = 1


    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #Case where the red/green is stored perfectly and I only need to push from that location for columns
    #green is loaded first full push
    if columnPosition[0] == 3:
        if TimeToUnload == 1 and PusherPosition != PusherStatus.READY:
          PusherPosition = outputSerial(serial_stepper, 'E')
        elif PusherPosition == PusherStatus.READY:
            robot.CollectionSystem.Pushers.UnLoadingPillarPusherTop()
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
            TimeToUnload = 3

    #red is loaded first   , green half push     
    else:
        if TimeToUnload == 1 and PusherPosition != PusherStatus.READY:
          PusherPosition = outputSerial(serial_stepper, 'D')
        elif PusherPosition == PusherStatus.READY:
            robot.CollectionSystem.Pushers.Half_UnLoadingPillarPusherTop()
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
            TimeToUnload = 2



    #red half push
    if TimeToUnload == 2 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'E')
    elif PusherPosition == PusherStatus.READY:
            robot.CollectionSystem.Pushers.Half_UnLoadingPillarPusherTop()
            robot.CollectionSystem.BraceSystem.BraceShake() #Loading and shaking brace
            TimeToUnload = 3

    if TimeToUnload == 3 and PusherPosition != PusherStatus.READY:
        PusherPosition = outputSerial(serial_stepper, 'B')
    elif PusherPosition == PusherStatus.READY:
        robot.CollectionSystem.BraceSystem.threeTowerset()
        robot.CollectionSystem.Pushers.UnloadingPillarPusherTop()
        robot.CollectionSystem.BraceSystem.threeTowerDrop()
        robot.CollectionSystem.BraceSystem.threeTowerClose()











    

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


    