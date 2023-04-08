import serial
import cv2
from time import time, sleep
from CameraSystem import CameraSystem
import Status


# Configure camera
camera = CameraSystem()


def main():
    with camera.camera as cam:
        while True:
            _, _, _ = camera.get_data()
            if cv2.waitKey(1) == 27:
                break
        cv2.destroyAllWindows()


if __name__ == '__main__':
    main()