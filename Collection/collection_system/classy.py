import cv2
import asyncio
from FlipperPlatform import FlipperPlatform, FlipperPlatformStatus
from CameraSystem import CameraSystem

def main():
    # Create objects
    camera = CameraSystem()
    flipper = FlipperPlatform()

    try:
        while True:
            elevator_data, middle_data, flipper_data = camera.get_data()
            print('Elevator Area: {}'.format(tuple(elevator_data)))
            print('Middle Area: {}'.format(tuple(middle_data)))
            print('Flipper Area: {}'.format(tuple(flipper_data)))

            if cv2.waitKey(1) == 27:
                break
            '''
            if not angles:
                flipper.set_status(FlipperPlatformStatus.NO_OBJECTS_IN_PLATFORM)

            else:
                flipper.set_current_angle(angles[0][1])
                if flipper.get_current_angle() > 15:
                    flipper.rotate_platform()
                else:
                    flipper.stop_rotation()
                    asyncio.run(flipper.flip_platform())
            '''
            
    # When the camera is unreachable, stop the program
    finally:
        cv2.destroyAllWindows()


if __name__ == '__main__':
  main()
