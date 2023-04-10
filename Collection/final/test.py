from time import sleep
import ElevatorPlatform
import PusherSystem
import Sweeper

ele = ElevatorPlatform.Elevator()
push = PusherSystem.Pushers()
sweep = Sweeper.Sweeper()
try:
    while True:
        sweep.reset_sweeper()

finally:
    sweep.reset_sweeper()