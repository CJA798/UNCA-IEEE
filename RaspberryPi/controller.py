import cv2
from time import time, sleep
import RPi.GPIO as GPIO
from CameraSystem import CameraSystem

GPIO.setmode(GPIO.BCM)
gpio_pins = [pin for pin in range(1,27)]
gpio_labels = []
for pin in gpio_pins:
    GPIO.setup(pin, GPIO.OUT) # set a port/pin as an output   
    GPIO.output(pin, 0)       # set port/pin value to 1/GPIO.HIGH/True 

camera = CameraSystem()

with camera.camera as cam:
    while True:
        elevator_data, mid_data, flipper_data = camera.get_data()
        #data_format: [(class_index, BoundingBox([x,y,w,h]), angle), ...]
        GPIO.output(7, 1)
        sleep(1)
        GPIO.output(7, 0)
        sleep(1)

        ''' TODO:
            1)  Trigger GPIO pins according to data gathered
                a)  if len(flipper_data) > 3 or len(elevator_data) > 3 or len(mid_data) > 0:
                        -   Put system in safe state: Stop rotating platforms, bring elevator platform down,
                            bring flipper platform to normal position, stop rotating storage drum, keep bracing status
                        -   Activate cleaning mechanism
                b)  Flipper platform:
                        - if the angle of the object in the flipper platform is not the target angle, set pin 1 to HIGH
                        - if the angle of the object is the target angle (or within the threshold), set pin 1 to LOW and pin 2 to HIGH
        '''



        if cv2.waitKey(1) == 27:
            break

cv2.destroyAllWindows()
GPIO.cleanup()
