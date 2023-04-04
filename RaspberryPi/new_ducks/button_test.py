from picamera2 import Picamera2

import RPi.GPIO as GPIO
import time

# Setup GPIO pins
GPIO.setmode(GPIO.BCM)

switch_pin = 18
led_pin = 23

GPIO.setup(switch_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(led_pin, GPIO.OUT)

# Set states
led_state = False
old_input_state = True # pulled-up

# Configure camera
picam2 = Picamera2()
capture_config = picam2.create_still_configuration()
picam2.start(show_preview=True)
time.sleep(1)
print("Preview started")


while True:
	frame = int(time.time())
	new_input_state = GPIO.input(switch_pin)
	if new_input_state == False and old_input_state == True:
		led_state = not led_state
		filename = '/home/pi/UNCA-IEEE/Collection/new_ducks/Pics/%03d.jpg' % frame
		picam2.switch_mode_and_capture_file(capture_config, filename)
		print ('Image captured: ' + filename)
		time.sleep(0.5)
		
		
	old_input_state = new_input_state
	GPIO.output(led_pin, led_state)
	frame += 1
