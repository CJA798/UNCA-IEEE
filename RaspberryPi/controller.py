import serial
import cv2
from time import time, sleep
from datetime import datetime 
from CameraSystem import CameraSystem
from functools import reduce
import Status



def main():
    # Debug variables
    time_program = True
    print_steps = True
    enable_serial = True
    if enable_serial:
        # Configure the serial port
        baud_rate = 115200
        ser = serial.Serial('/dev/ttyACM0', baud_rate)
        ser.reset_input_buffer()

    # Configure robot
    robot_status = Status.RobotStatus()

    # Configure camera
    camera = CameraSystem()

    # General status variables
    navigation_status = robot_status.navigation_status
    collection_status = robot_status.collection_status
    stored_items = []

    # 16-bit status variables
    camera_working = robot_status.camera_status
    intake_status = robot_status.intake_status
    flipper_status = robot_status.flipper_status
    sweeper_status = robot_status.sweeper_status
    elevator_status = robot_status.elevator_status
    top_pusher_status = robot_status.top_pusher_status
    bot_pusher_status = robot_status.bot_pusher_status
    brace_status = robot_status.brace_status

    # Threshold variables
    MAX_ANGLE_FLIPPER = 100
    MIN_ANGLE_FLIPPER = 70

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
            if time_program: start_time = time()
            if print_steps: print('+--------------------------BEGIN----------------------------+')
            elevator_data, mid_data, flipper_data = camera.get_data()
            #data_format: [(class_index, BoundingBox([x,y,w,h]), angle), ...]

            # If there are no objects, set respective EMPTY or RESTING status
            
            if not mid_data:
                sweeper_status = Status.SweeperStatus.SWEEPER_RESTING
                if print_steps: print('Sweeper resting')
            

            # If there's more than one object in the elevator or the flipper, or an object in the middle, activate sweeper 
            if any(len(data) > 1 for data in [flipper_data, elevator_data]) or len(mid_data) > 0:
                # TODO: Put system in safe state: Stop rotating platforms, bring elevator platform down,
                #       bring flipper platform to normal position, stop rotating storage drum, keep bracing status
                #       if the system is safe to clean, activate cleaning mechanism

                intake_status = Status.IntakeStatus.INTAKE_OFF
                flipper_status = Status.FlipperStatus.FLIPPER_CLEANING
                elevator_status = Status.ElevatorStatus.ELEVATOR_CLEANING
                top_pusher_status = Status.PusherStatus.TOP_PUSHER_RETRACTED
                bot_pusher_status = Status.PusherStatus.BOT_PUSHER_RETRACTED
                
                # TODO: If system is in safe status, activate sweeper
                sweeper_status = Status.SweeperStatus.SWEEPER_EJECTING


            else:
                ''' Flipper '''
                # If there are no objects in the flipper platform, set flipper status to EMPTY
                if not flipper_data:
                    flipper_status = Status.FlipperStatus.FLIPPER_EMPTY
                    if print_steps: print('Flipper empty')
                # If the angle data is missing, ignore reading and keep previous status
                elif len(flipper_data[0]) < 3:
                    if print_steps: print('Keeping previous flipper value')
                    pass
                # If the angle of the object in the flipper platform is not the target angle, set flipper status to ROTATE
                elif flipper_data[0][2] > MAX_ANGLE_FLIPPER or flipper_data[0][2] < MIN_ANGLE_FLIPPER: 
                    flipper_status = Status.FlipperStatus.FLIPPER_ORIENTING
                    if print_steps: print('Flipper orienting')
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
                    elevator_status = Status.ElevatorStatus.ELEVATOR_EMPTY
                    if print_steps: print('Elevator empty')
                # If the angle data is missing, ignore reading and keep previous status
                elif len(elevator_data[0]) < 3:
                    if print_steps: print('Keeping previous elevator value')
                    pass
                # If the angle of the object in the elevator platform is not the target angle, set elevator status to ROTATE
                elif elevator_data[0][2] > MAX_ANGLE_ELEVATOR or elevator_data[0][2] < MIN_ANGLE_ELEVATOR:
                    if print_steps: print('Elevator orienting')
                    elevator_status = Status.ElevatorStatus.ELEVATOR_ORIENTING
                # If the angle of the object in the elevator platform is within the allowed threshold, set elevator status to RAISING and sweeper to PUSHING*
                else:
                    elevator_status = Status.ElevatorStatus.ELEVATOR_RAISING
            
            data_package = [camera_working, intake_status, flipper_status, sweeper_status,
                        elevator_status, top_pusher_status, bot_pusher_status, brace_status]
            
            int_data = sum(data_package)
            print(int_data)

            if enable_serial:
                if int_data >= 32768:
                    timestamp = time()
                    date_time = datetime.fromtimestamp(timestamp)
                    print(f"{date_time} -> {bin(int_data)}")
                    ser.reset_output_buffer()
                    str_data = str(int_data) + "\n"
                    ser.write(str_data.encode())
                else:
                    print("ERROR: ENCODED DATA IS NOT 16-BITS")

            if time_program: 
                end_time = time()
                total_time = end_time - start_time
                print(f"Total time taken: {total_time} seconds")

            if cv2.waitKey(1) == 27:
                break

    cv2.destroyAllWindows()

    # If the camera is not working, send respective flag and start restart routine

    encoded_data = 0b0_00_00_00_00_000
    if enable_serial:
        ser.reset_output_buffer()
        ser.write()

    print("ERROR: CAMERA NOT DETECTED")
    print("restarting...")
    # TODO: Start restart routine



if __name__ == '__main__':
    main()