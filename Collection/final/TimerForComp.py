import time

timer = 180  # seconds
while timer:
    mins, secs = divmod(timer, 60)
    time_str = f"{mins:02d}:{secs:02d}"
    print(time_str, end="\r")
    time.sleep(1)
    timer -= 1
print("Time's up!")
