import serial
import cv2
from time import time, sleep
from CameraSystem import CameraSystem
import Status



def main():
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

    # Arrays for data encoding
    variable_status = [camera_working, intake_status, flipper_status, sweeper_status,
                        elevator_status, top_pusher_status, bot_pusher_status, brace_status]
    bits_per_variable = [1, 2, 2, 2, 2, 2, 2, 3]

    # Threshold variables
    MAX_ANGLE_FLIPPER = 90
    MIN_ANGLE_FLIPPER = 80

    MAX_ANGLE_ELEVATOR = 90
    MIN_ANGLE_ELEVATOR = 80

    with camera.camera as cam:
        # Keep waiting for navigation teensy to sent start collection signal
        #while navigation_status == Status.NavigationStatus.START:
        #    navigation_status = ser.readline().decode('utf-8').rstrip()
        # TODO: Send signal back saying collection system is ON
        # navigation teensy should keep sending start signal until pi replies back

        intake_status = Status.IntakeStatus.INTAKE_ON
        # TODO: Assign actual status
        brace_status = Status.BraceStatus.A

        while True:
            elevator_data, mid_data, flipper_data = camera.get_data()
            #data_format: [(class_index, BoundingBox([x,y,w,h]), angle), ...]

            # If there are no objects, set respective EMPTY or RESTING status
            
            if not mid_data:
                sweeper_status = Status.SweeperStatus.RESTING
            

            # If there's more than one object in the elevator or the flipper, or an object in the middle, activate sweeper 
            if any(len(data) > 3 for data in [flipper_data, elevator_data]) or len(mid_data) > 0:
                # TODO: Put system in safe state: Stop rotating platforms, bring elevator platform down,
                #       bring flipper platform to normal position, stop rotating storage drum, keep bracing status
                #       if the system is safe to clean, activate cleaning mechanism
                pass

            else:
                ''' Flipper '''
                # If there are no objects in the flipper platform, set flipper status to EMPTY
                if not flipper_data:
                    flipper_status = Status.FlipperStatus.EMPTY
                # If the angle of the object in the flipper platform is not the target angle, set flipper status to ROTATE
                elif flipper_data[0][3] > MAX_ANGLE_FLIPPER or flipper_data[0][3] < MIN_ANGLE_FLIPPER: 
                    flipper_status = Status.FlipperStatus.ORIENTING
                # If the angle of the object in the flipper platform is within the allowed threshold, set flipper status to FLIPPING
                else:
                    # TODO: Run template matching to determine whether the object is already in the right position or not
                    # if object_matches:
                    #     sweeper_status = Status.SweeperStatus.PUSHING  
                    #else:
                    #    flipper_status = Status.FlipperStatus.FLIPPING  
                    pass

                ''' Elevator '''
                # If there are no objects in the elevator platform, set elevator status to EMPTY
                if not elevator_data:
                    elevator_status = Status.ElevatorStatus.EMPTY
                # If the angle of the object in the elevator platform is not the target angle, set elevator status to ROTATE
                elif elevator_data[0][3] > MAX_ANGLE_ELEVATOR or elevator_data[0][3] < MIN_ANGLE_ELEVATOR:
                    elevator_status = Status.ElevatorStatus.ORIENTING
                # If the angle of the object in the elevator platform is within the allowed threshold, set elevator status to ELEVATING and sweeper to PUSHING*
                else:
                    pass

            ''' TODO:
                1)  Trigger GPIO pins according to data gathered
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

            
            

            # Convert variables to binary strings and pad with zeros
            binary_vars = [format(var, 'b').zfill(bits) for var, bits in zip(variable_status, bits_per_variable)]

            # Concatenate binary strings and convert back to integer
            encoded_data = sum(int(binary, 2) for binary in binary_vars)
            print(encoded_data)
            #ser.reset_output_buffer()
            #ser.write(encoded_data.encode())

            if cv2.waitKey(1) == 27:
                break

    cv2.destroyAllWindows()

    # If the camera is not working, send respective flag and start restart routine

    encoded_data = 0b0_00_00_00_00_000
    print("ERROR: CAMERA NOT DETECTED")
    print("restarting...")
    #ser.reset_output_buffer()
    #ser.write(encoded_data.encode())
    # TODO: Start restart routine



if __name__ == '__main__':
    main()