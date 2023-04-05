import cv2
from time import time
import RPi.GPIO as GPIO
from CameraSystem import CameraSystem

camera = CameraSystem()

with camera.camera as cam:
    while True:
        start_time = time()
        elevator_data, mid_data, flipper_data = camera.get_data()
        if cv2.waitKey(1) == 27:
            break
        end_time = time()
        cv_time = end_time - start_time
        print("Total time: {}".format(cv_time))
    cv2.destroyAllWindows()
