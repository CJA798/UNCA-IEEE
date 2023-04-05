import cv2
import numpy as np
from typing import List, Tuple
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
        self._frame_width = 800
        self._frame_height = 480
        self._num_threads = 4
        self._enable_edgetpu = True
        self._min_area = 0.02 * self._frame_width * self._frame_height
        self._max_area = self._frame_width * self._frame_height
        self._max_results = 10
        self._score_threshold = 0.7
        self.camera = Picamera2()
        self._preview_config = self.camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (self._frame_width, self._frame_height)})
       
        self.camera.configure(self._preview_config)
        self.camera.start()
        self.initialize_detector()
    

    def initialize_detector(self):
        base_options = core.BaseOptions(file_name=self._model, use_coral=self._enable_edgetpu, num_threads=self._num_threads)
        detection_options = processor.DetectionOptions(max_results=self._max_results, score_threshold=self._score_threshold)
        options = vision.ObjectDetectorOptions(base_options=base_options, detection_options=detection_options)
        self._detector = vision.ObjectDetector.create_from_options(options)


    def run_inference(self, image: np.ndarray):
        # Convert the image from BGR to RGB as required by the TFLite model.
        rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # Create a TensorImage object from the RGB image.
        input_tensor = vision.TensorImage.create_from_array(rgb_image)

        # Run object detection estimation using the model.
        detection_result = self._detector.detect(input_tensor)
        
        return detection_result


    def get_angles(self, image, bounding_boxes):
        # Convert oriented_image to hsv to identify colors easily
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
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

            biggest_contour = None
            biggest_area = 0
            offset_biggest_contour = None
            for bbox in bounding_boxes:
            # Extract the region of the image defined by the bounding box
                x, y, w, h = bbox
                bw_roi = bw[y:y+h, x:x+w]
                if bw_roi.shape[0] > 0 and bw_roi.shape[1] > 0:
                    cv2.imshow('B/W Region of Interest', bw_roi)
                
                # Find the contours in the binary image
                contour, _ = cv2.findContours(bw_roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)


                # Enumerate contours
                for _, c in enumerate(contour):
                    # Calculate the area of each contour
                    area = cv2.contourArea(c)

                    # Ignore contours that are too small or too large
                    if area < self._min_area or area > self._max_area:
                        continue

                    if area > biggest_area:
                        biggest_area = area
                        biggest_contour = c
                        offset_biggest_contour = biggest_contour + np.array([x, y])

                    # Draw the contours on image
                    cv2.drawContours(image, offset_biggest_contour, -1, (255,255,0), 3)
                    
                    # Find the orientation of each shape
                    angle = utils.getOrientation(biggest_contour, image, x, y)
                    
                    contours.append(offset_biggest_contour)  # Shift contour points back to the original image coordinates
                    angles.append(angle)


        return angles


    def summarize_data(self, detections, angles):
        ''' This function summarizes the data from run_inference
            and get_angles in a list'''
        objects_data = []
        if len(detections) != len(angles):
            print("The number of detections and angles doesn't match.")
            return objects_data


        for detection, angle in zip(detections, angles):
            bbox = (detection.bounding_box)
            category_index = detection.categories[0].index
            objects_data.append((category_index, bbox, angle))
        
        return objects_data


    def draw_areas(self, image):
        # Define the margin size and color
        margin_size = 50
        margin_color = (0, 255, 0)  # blue
        
        # Get image dimensions
        height, width = image.shape[:2]

        # Draw the margin rectangle
        cv2.rectangle(image, (margin_size, margin_size), (width - margin_size, height - margin_size), margin_color, thickness=2)

        # Define the vertical lines positions and color
        line_pos1 = 4 * (width // 7)
        line_pos2 = 6 * (width // 9)
        line_color = (0, 255, 0)  # green
        
        # Draw the vertical lines
        cv2.line(image, (line_pos1, margin_size), (line_pos1, height - margin_size), line_color, thickness=2)
        cv2.line(image, (line_pos2, margin_size), (line_pos2, height - margin_size), line_color, thickness=2)

        # Add labels to the areas
        font = cv2.FONT_HERSHEY_SIMPLEX
        font_scale = 1
        label_color = (0, 255, 0)  # red
        
        # Left area (Elevator)
        left_label_pos = (margin_size + 10, height // 10)
        cv2.putText(image, "Elevator", left_label_pos, font, font_scale, label_color, thickness=2)

        # Middle area
        middle_label_pos = (line_pos1 + 10, height // 10)
        cv2.putText(image, "Mid", middle_label_pos, font, font_scale, label_color, thickness=2)

        # Right area (Flipper)
        right_label_pos = (line_pos2 + 10, height // 10)
        cv2.putText(image, "Flipper", right_label_pos, font, font_scale, label_color, thickness=2)

        return image


    def split_data(self, data):
        ''' This function splits the data into three lists
            depending on the objects position.
            Each list corresponds to an area:
                -   Elevator
                -   Middle
                -   Flipper'''
        elevator_data = []
        middle_area_data = []
        flipper_data = []
        forbidden_area_start = self._frame_width // 5
        forbidden_area_end = (self._frame_width * 3) // 5
        for item in data:
            x_coord = item[1].origin_x
            if x_coord < forbidden_area_start:
                elevator_data.append(item)
            else:
                flipper_data.append(item)
            #else:
                #middle_area_data.append(item)

        return elevator_data, middle_area_data, flipper_data


    def get_data(self):
        ''' This function runs the computer vision routine and
        returns three lists: elevator, middle, and flipper.'''  
        # Take picture
        image = self.camera.capture_array()
        #image = cv2.flip(image, 1)

        # Run inference
        detection_result = self.run_inference(image)
        detection_result = detection_result.detections
        
        # Sort detections from left to right
        detection_result = sorted(detection_result, key=lambda d: d.bounding_box.origin_x)

        # Extract the bounding boxes from the DetectionResult object
        bounding_boxes = [(d.bounding_box.origin_x, d.bounding_box.origin_y, d.bounding_box.width, d.bounding_box.height) for d in detection_result]
        angles = self.get_angles(image, bounding_boxes)

        # Summarize data in a single list
        object_data = self.summarize_data(detection_result, angles)
        elevator_area, middle_area, flipper_area = self.split_data(object_data)
        
        # Draw keypoints and edges on input image
        image = utils.visualize(image, detection_result)
        image = self.draw_areas(image)
        # Stop the program if the ESC key is pressed.
        cv2.imshow('object_detector', image)
        #cv2.imshow('HSV', hsv)
        #cv2.imshow('Gray', gray).
    
        print('Elevator Area: {}'.format(tuple(elevator_area)))
        print('Middle Area: {}'.format(tuple(middle_area)))
        print('Flipper Area: {}'.format(tuple(flipper_area)))

        return elevator_area, middle_area, flipper_area