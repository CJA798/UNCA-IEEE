# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

#
# NOTE - Only for use on Raspberry Pi or other SBC.
#

# Simple two DC motor robot class.  Exposes a simple LOGO turtle-like API for
# moving a robot forward, backward, and turning.  See RobotTest.py for an
# example of using this class.
# Author2: Tony DiCola, Chris Anderson
# License: MIT License https://opensource.org/licenses/MIT


# This assumes the Left motor is on Motor 1 and the Right motor is on Motor 2


import time
import atexit
import board
from pykalman import KalmanFilter
import numpy as np
from adafruit_motorkit import MotorKit
from adafruit_lsm6ds.ism330dhcx import ISM330DHCX
from adafruit_vl53l4cd import VL53L4CD

i2c = board.I2C()

class Robot:
    def __init__(self, left_motor_trim=0, right_motor_trim=0, stop_at_exit=True):
        """Create an instance of the robot.  Can specify the following optional
        parameter
         - left_motor_trim: Amount to offset the speed of the left motor, can be positive
                            or negative and use useful for matching the speed of both
                            motors.  Default is 0.
         - right_motor_trim: Amount to offset the speed of the right motor (see above).
         - stop_at_exit: Boolean to indicate if the motors should stop on program
                         exit.  Default is True (highly recommended to keep this
                         value to prevent damage to the bot on program crash!).
        """

        # Initial Position
        self.position = (0,0)           # Initial position (x,y)
        self.orientation = 0            # Initial orientation (degrees)

        # Sensors and Shields
        self.kit = MotorKit(i2c)        # Motor Kit: Adafruit DC and Stepper Motor HAT for Raspberry Pi
        self.imu = ISM330DHCX(i2c)      # IMU: Inertial Measurement Unit
        self.tof = VL53L4CD(i2c)        # ToF: Time of Flight

        ## Kalman Filter   
        AccX_Value = self.imu.acceleration[2]
        AccX_Variance = 0.0020

        # time step
        dt = 0.01

        # transition_matrix  
        F = [[1, dt, 0.5*dt**2], 
            [0,  1,       dt],
            [0,  0,        1]]

        # observation_matrix   
        H = [0, 0, 1]

        # transition_covariance 
        Q = [[0.2,    0,      0], 
            [  0,  0.1,      0],
            [  0,    0,  10e-4]]

        # observation_covariance 
        R = AccX_Variance

        # initial_state_mean
        X0 = [0,
             0,
             AccX_Value]

        # initial_state_covariance
        P0 = [[  0,    0,               0], 
             [  0,    0,               0],
             [  0,    0,   AccX_Variance]]

        filtered_state_mean = X0
        filtered_state_covariance = P0

        self.kf = KalmanFilter( transition_matrices = F, 
                                observation_matrices = H, 
                                transition_covariance = Q, 
                                observation_covariance = R, 
                                initial_state_mean = X0, 
                                initial_state_covariance = P0)

        # Motor Trim
        self._left_motor_trim = left_motor_trim
        self._right_motor_trim = right_motor_trim

        # Stop on program exit
        if stop_at_exit:
            atexit.register(self.stop)

    def _left_speed(self, speed):
        """Set the speed of the left motor, taking into account its trim offset."""
        assert -1 <= speed <= 1, "Speed must be a value between -1 to 1 inclusive!"
        speed += self._left_motor_trim
        speed = max(-1, min(1, speed))  # Constrain speed to 0-255 after trimming.
        self.kit.motor1.throttle = speed

    def _right_speed(self, speed):
        """Set the speed of the right motor, taking into account its trim offset."""
        assert -1 <= speed <= 1, "Speed must be a value between -1 to 1 inclusive!"
        speed += self._right_motor_trim
        speed = max(-1, min(1, speed))  # Constrain speed to 0-255 after trimming.
        self.kit.motor2.throttle = speed

    @staticmethod
    def stop(self):
        """Stop all movement."""
        self.kit.motor1.throttle = 0
        self.kit.motor2.throttle = 0

    def steer(self, speed, direction):
        # Move forward at the specified speed (0- 1).  Direction is +- 1.
        # Full left is -1, Full right is +1
        if (speed + direction / 2) > 1:
            speed = (
                speed - direction / 2
            )  # calibrate so total motor output never goes above 1
        left = speed + direction / 2
        right = speed - direction / 2
        self._left_speed(left)
        self._right_speed(right)

    def _calculate_position(self):
        self.filtered_state_mean, self.filtered_state_covariance = (
        self.kf.filter_update(
            self.filtered_state_mean,
            self.filtered_state_covariance,
            self.AccX_Value
        ))
        self.position = (self.filtered_state_mean[0], 0)

    def get_position(self):
        return self.position