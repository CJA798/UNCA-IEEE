from time import sleep
from pi_servo_hat import PiServoHat

# Instantiate the object
hat = PiServoHat()

# Set the PWM frequency to 50Hz
hat.set_pwm_frequency(200)

for i in range(0,100):
    hat.move_servo_position(0, i)
    print(i)
    sleep(0.25)