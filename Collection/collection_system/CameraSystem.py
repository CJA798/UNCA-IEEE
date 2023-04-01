import cv2
import numpy as np
from picamera2 import Picamera2
from tflite_support.task import core, processor, vision
from math import atan2, cos, sin, sqrt, pi
import utils
import asyncio


class CameraSystem:
    def __init__(self):
        self._model = 'improv_12k_00EfficientDet_edgetpu.tflite'
        #self._camera_id = 0
        #self._frame_width = 240
        #self._frame_height = 120
        self._frame_width = 640
        self._frame_height = 480
        self._num_threads = 4
        self._enable_edgetpu = True
        self._min_area = 0.05 * self._frame_width * self._frame_height
        self._max_area = self._frame_width * self._frame_height
        self._max_results = 10
        self._score_threshold = 0.6
        self.camera = Picamera2()
        self._preview_config = self.camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (frame_width, frame_height)})
       
        self.camera.configure(self._preview_config)
        self.camera.start()
        self.initialize_detector()
    

    def initialize_detector(self):
        base_options = core.BaseOptions(file_name=self._model, use_coral=self.enable_edgetpu, num_threads=self.num_threads)
        detection_options = processor.DetectionOptions(max_results=self._max_results, score_threshold=self._score_threshold)
        options = vision.ObjectDetectorOptions(base_options=base_options, detection_options=detection_options)
        self._detector = vision.ObjectDetector.create_from_options(options)


    def get_data(self):    
        # Continuously capture images from the camera and run inference
        # Take picture
        image = self.camera.capture_array()

        # OBJECT DETECTION:
        image = cv2.flip(image, 1)

        # Duplicate image. First one, for object detection; second one, for object orientation.
        oriented_image = image

        # Convert the image from BGR to RGB as required by the TFLite model.
        rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # Create a TensorImage object from the RGB image.
        input_tensor = vision.TensorImage.create_from_array(rgb_image)

        # Run object detection estimation using the model.
        detection_result = self._detector.detect(input_tensor)

        # Extract the indexes from the DetectionResult object
        class_index = [d.categories[0].index for d in detection_result.detections]


        # OBJECT ORIENTATION:
        # Extract the bounding boxes from the DetectionResult object
        bounding_boxes = [(d.bounding_box.origin_x, d.bounding_box.origin_y, d.bounding_box.width, d.bounding_box.height) for d in detection_result.detections]

        # Convert oriented_image to hsv to identify colors easily
        hsv = cv2.cvtColor(oriented_image, cv2.COLOR_BGR2HSV)
        # Convert oriented_image to grayscale to identify white
        h, _, gray = cv2.split(hsv)
        # Convert oriented_image to grayscale
        #gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY)

        # Convert oriented_image to binary
        _, bw = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

        # Find the contours of each bounding box
        contours = []
        angles = []
        for bbox in bounding_boxes:
            # Extract the region of the image defined by the bounding box
            x, y, w, h = bbox
            bw_roi = bw[y:y+h, x:x+w]
            if bw_roi.shape[0] > 0 and bw_roi.shape[1] > 0:
                cv2.imshow('B/W Region of Interest', bw_roi)
            
            # Find the contours in the binary image
            contour, _ = cv2.findContours(bw_roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

            # Enumerate contours
            for i, c in enumerate(contour):
                # Calculate the area of each contour
                area = cv2.contourArea(c)

                # Ignore contours that are too small or too large
                if area < self._min_area or area > self._max_area:
                    continue

                # Draw the contours on image
                cv2.drawContours(image, contour[i]+ np.array([x, y]), -1, (255,255,0), 3)
                
                # Find the orientation of each shape
                angle = utils.getOrientation(c, image, x, y)
                
                contours.append(contour[0] + np.array([x, y]))  # Shift contour points back to the original image coordinates
                angles.append(angle)
        
        # Draw keypoints and edges on input image
        image = utils.visualize(image, detection_result)
        
        # Stop the program if the ESC key is pressed.
        cv2.imshow('object_detector', image)
        #cv2.imshow('HSV', hsv)
        #cv2.imshow('Gray', gray)

        # Zip the two data arrays together into one iterable
        object_data = zip(class_index,angles)
        # Convert to list to index data
        object_data = list(object_data)
        
        return object_data
