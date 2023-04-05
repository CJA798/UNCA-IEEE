import cv2
from time import time, sleep
import RPi.GPIO as GPIO
from CameraSystem import CameraSystem

GPIO.setmode(GPIO.BCM)
gpio_pins = [pin for pin in range(1,27)]
gpio_labels = []
for pin in gpio_pins:
    GPIO.setup(pin, GPIO.OUT) # set a port/pin as an output
    print("Pin {} Set to OUTPUT".format(pin))   
    GPIO.output(pin, 0)       # set port/pin value to 1/GPIO.HIGH/True
    print("Pin {} Set to LOW".format(pin))

camera = CameraSystem()

with camera.camera as cam:
    while True:
        #elevator_data, mid_data, flipper_data = camera.get_data()
        #data_format: [(class_index, BoundingBox([x,y,w,h]), angle), ...]
        GPIO.output(21, 1)
        GPIO.output(20, 1)
        print("ON")
        sleep(4)
        GPIO.output(21, 0)
        GPIO.output(20, 0)
        print("OFF")
        sleep(4)

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



        if cv2.waitKey(1) == 27:
            break

cv2.destroyAllWindows()
GPIO.cleanup()
