from time import sleep
from pi_servo_hat import PiServoHat

# Instantiate the object
hat = PiServoHat()
hat.restart()
# Set the PWM frequency to 50Hz
hat.set_pwm_frequency(50)

for i in range(0,180):
    hat.move_servo_position(0, i, 180)
    print(i)
    sleep(0.1)
