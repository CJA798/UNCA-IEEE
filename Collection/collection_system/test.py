from time import sleep
from pi_servo_hat import PiServoHat

# Instantiate the object
hat = PiServoHat()
hat.restart()
# Set the PWM frequency to 50Hz
hat.set_pwm_frequency(50)

channel = 1
swing = 180

hat.move_servo_position(channel, 0, swing)
print("0, {}".format(swing))
sleep(2)
hat.move_servo_position(channel, 240, swing)
print("180, {}".format(swing))
sleep(2)

