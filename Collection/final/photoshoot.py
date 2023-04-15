#!/usr/bin/env python3
from FlipperPlatform import Flipper
from libcamera import controls
from picamera2 import Picamera2, Preview
import time
import cv2



flipper = Flipper()
flipper.stop_rotation()

with Picamera2() as picam2:
    frame = int(time.time())

    # Set up QT preview window.
    picam2.start_preview(Preview.QT)
    preview_config = picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (800, 480)})
    capture_config = picam2.create_still_configuration(main={"format": 'XRGB8888', "size": (800, 480)})
    picam2.configure(preview_config)
    picam2.start()
    time.sleep(1)
    print("Preview started")

    # Turn on full-time autofocus.
    picam2.set_controls({"AfMode": 2 ,"AfTrigger": 0})

    # Wait for button press. When pressed, take picture.
    while True:
        filename = '/home/pi/Pictures/Push/%03d.jpg' % frame
        image = picam2.switch_mode_and_capture_array(capture_config)
        cropped_img = image[2*480//7:480, 3*800//5:, :]
        # Save the cropped image in /home/pi/Pictures
        cv2.imwrite(filename, cropped_img)
        print ('Image captured: ' + filename)
        frame += 1
        flipper.rotate_platform()
        time.sleep(0.5)
        flipper.stop_rotation()
