import CameraSystem
import cv2
camera = CameraSystem.CameraSystem()

try:
    with camera.camera as cam:
        
        while True:
            elevator_data, mid_data, flipper_data = camera.get_data()
            camera.is_duck_standing3()
            if cv2.waitKey(1) == 27:
                break

        cv2.destroyAllWindows()
# When the camera is unreachable, stop the program
finally:
    cv2.destroyAllWindows()
