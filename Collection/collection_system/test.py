from time import sleep
from pi_servo_hat import PiServoHat

# Instantiate the object
servo_hat = PiServoHat()
servo_hat.restart()

# Set the PWM frequency to 50Hz
servo_hat.set_pwm_frequency(50)

# Set the servo channel to use
channel = 14
swing = 180

# Define the servo positions
min_position = 0
max_position = 240

'''
Ducks height = -110
Columns height = 0
Down = 230
'''


servo_hat.move_servo_position(channel, 0, swing)
#sleep(1)

#for i in range(0, 211, 1):
 #   servo_hat.move_servo_position(channel, i, swing)
  #  sleep(0.1)

'''
for i in 
servo_hat.set_pwm_frequency(servo_channel, i)
'''
