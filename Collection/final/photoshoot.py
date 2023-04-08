#!/usr/bin/env python3
from libcamera import controls
from picamera2 import Picamera2, Preview
import time


with Picamera2() as picam2:
    frame = int(time.time())

    # Set up QT preview window.
    picam2.start_preview(Preview.QT)
    preview_config = picam2.create_preview_configuration()
    capture_config = picam2.create_still_configuration()
    picam2.configure(preview_config)
    picam2.start()
    time.sleep(1)
    print("Preview started")

    # Turn on full-time autofocus.
    picam2.set_controls({"AfMode": 2 ,"AfTrigger": 0})

    # Wait for button press. When pressed, take picture.
    while True:
        filename = '/home/pi/Pictures/%03d.jpg' % frame
        picam2.switch_mode_and_capture_file(capture_config, filename)
        print ('Image captured: ' + filename)
        frame += 1
        time.sleep(1)
