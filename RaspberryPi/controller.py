import serial
import cv2
from time import time, sleep
from CameraSystem import CameraSystem
import Status


# Configure the serial port
#baud_rate = 115200
#ser = serial.Serial('/dev/ttyACM0', baud_rate)
#ser.reset_input_buffer()

# Configure robot
robot_status = Status.RobotStatus()

# Configure camera
camera = CameraSystem()

# General status variables
navigation_status = robot_status.navigation_status
collection_status = robot_status.collection_status

# 16-bit status variables
camera_working = True
intake_status = robot_status.intake_status
flipper_status = robot_status.flipper_status
sweeper_status = robot_status.sweeper_status
elevator_status = robot_status.elevator_status
top_pusher_status = robot_status.top_pusher_status
bot_pusher_status = robot_status.bot_pusher_status
brace_status = robot_status.brace_status


def encode_data(elevator_data: list, mid_data: list, flipper_data: list):
    

    if len(flipper_data) > 3 or len(elevator_data) > 3 or len(mid_data) > 0:
        pass
    binary_data = ''
    return binary_data

with camera.camera as cam:
    #while navigation_status == Status.NavigationStatus.START:
    #    navigation_status = ser.readline().decode('utf-8').rstrip()
    intake_status = Status.IntakeStatus.INTAKE_ON
    
    while True:
        elevator_data, mid_data, flipper_data = camera.get_data()
        #data_format: [(class_index, BoundingBox([x,y,w,h]), angle), ...]
        encoded_data = encode_data(elevator_data, mid_data, flipper_data)
        ''' TODO:
            1)  Trigger GPIO pins according to data gathered
                a)  if len(flipper_data) > 3 or len(elevator_data) > 3 or len(mid_data) > 0:
                        -   Put system in safe state: Stop rotating platforms, bring elevator platform down,
                            bring flipper platform to normal position, stop rotating storage drum, keep bracing status
                        -   Activate cleaning mechanism
                b)  Intake Mechanism:
                        -   Send through serial line 1
                c)  Flipper platform:
                        -   if the angle of the object in the flipper platform is not the target angle, set FLIPPER_ROTATING_PLATFORM to HIGH
                        -   if the angle of the object is the target angle (or within the threshold):
                                -   if the object is a duck and is already in storage position, set BRUSH to HIGH 
                                -   else, set FLIPPER_ROTATING_PLATFORM to LOW and FLIPPER to HIGH* 
                d)  Elevator Platform, Storage Drum, and Pushers:
                        -   if the angle of the object in the elevator platform is not the target angle, set ELEVATOR_ROTATING_PLATFORM to HIGH
                        -   if the angle of the object is the target angle (or within the threshold):
                                -   if the height is not the target height, set ELEVATOR to HIGH
                                -   else set ELEVATOR to LOW:


                
        '''

        encoded_data = ''
        #ser.reset_output_buffer()
        #ser.write(encoded_data.encode())

        if cv2.waitKey(1) == 27:
            break

cv2.destroyAllWindows()

