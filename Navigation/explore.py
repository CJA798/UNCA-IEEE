import board

from robot import Robot
from followermachine import FollowerMachine


LEFT_TRIM = 0
RIGHT_TRIM = 0

# Instantiate communication protocol 
i2c=board.I2C()
print("i2c ready")

# Instantiate robot and fsm objects
robot = Robot(left_trim=LEFT_TRIM, right_trim=RIGHT_TRIM)
#fsm = FollowerMachine()
print("Robot and FSM ready")


def main():
    robot._calculate_position()
    print(robot.get_position())


if __name__ == '__main__':
    while True:
        main()