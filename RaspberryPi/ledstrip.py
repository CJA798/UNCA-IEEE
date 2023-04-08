import RPi.GPIO as GPIO
import time


# set up GPIO mode and pin number
GPIO.setmode(GPIO.BCM)
led_pin = 27

# set up the LED pin as an output
GPIO.setup(led_pin, GPIO.OUT)

# loop forever
while True:
    # turn the LED on
    GPIO.output(led_pin, GPIO.HIGH)
    time.sleep(2)  # wait for 2 seconds

    # turn the LED off
    GPIO.output(led_pin, GPIO.LOW)
    time.sleep(1)  # wait for 3 seconds

GPIO.cleanup()
