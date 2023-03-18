import argparse
import sys
import time

import cv2
from picamera2 import Picamera2
from tflite_support.task import core
from tflite_support.task import processor
from tflite_support.task import vision
import utils

from math import atan2, cos, sin, sqrt, pi
import numpy as np

import serial
import signal

'''TODO 
    1) Send respective string when no objects detected
    2) Don't send data until Teensy has processed the previous data package
    3) Figure out why the Teensy is slow '''


# Configure the serial port
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.reset_input_buffer()

# Safe Arduino exit on keyboard interrupt
def arduino_exit(signal, frame):
    ser.write(b'EXIT')  # send exit message to Arduino
    sys.exit(0)

signal.signal(signal.SIGINT, arduino_exit)

def run(model: str, camera_id: int, width: int, height: int, num_threads: int,
        enable_edgetpu: bool) -> None:
  """Continuously run inference on images acquired from the camera.

  Args:
    model: Name of the TFLite object detection model.
    camera_id: The camera id to be passed to OpenCV.
    width: The width of the frame captured from the camera.
    height: The height of the frame captured from the camera.
    num_threads: The number of CPU threads to run the model.
    enable_edgetpu: True/False whether the model is a EdgeTPU model.
  """

  # Initialize the object detection model
  base_options = core.BaseOptions(
      file_name=model, use_coral=enable_edgetpu, num_threads=num_threads)
  detection_options = processor.DetectionOptions(
      max_results=10, score_threshold=0.7)
  options = vision.ObjectDetectorOptions(
      base_options=base_options, detection_options=detection_options)
  detector = vision.ObjectDetector.create_from_options(options)

  with Picamera2() as picam2:
    # Configure camera mode
    preview_config = picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (160, 120)})
    picam2.configure(preview_config)
    picam2.start()
    
    while True:
      # Continuously capture images from the camera and run inference
      # Take picture
      image = picam2.capture_array()

      # OBJECT DETECTION:
      image = cv2.flip(image, 1)

      # Duplicate image. First one, for object detection; second one, for object orientation.
      oriented_image = image

      # Convert the image from BGR to RGB as required by the TFLite model.
      rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

      # Create a TensorImage object from the RGB image.
      input_tensor = vision.TensorImage.create_from_array(rgb_image)

      # Run object detection estimation using the model.
      detection_result = detector.detect(input_tensor)

      # Extract the indexes from the DetectionResult object
      class_index = [d.categories[0].index for d in detection_result.detections]


      # OBJECT ORIENTATION:
      # Extract the bounding boxes from the DetectionResult object
      bounding_boxes = [(d.bounding_box.origin_x, d.bounding_box.origin_y, d.bounding_box.width, d.bounding_box.height) for d in detection_result.detections]

      # Convert oriented_image to hsv to identify colors easily
      hsv = cv2.cvtColor(oriented_image, cv2.COLOR_BGR2HSV)
      # Convert oriented_image to grayscale to identify white
      h, s, gray = cv2.split(hsv)
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
          cv2.imshow('B/W Region of Interest', bw_roi)

          # Find the contours in the binary image
          contour, _ = cv2.findContours(bw_roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

          # Enumerate contours
          for i, c in enumerate(contour):
            # Calculate the area of each contour
            area = cv2.contourArea(c)

            # Ignore contours that are too small or too large
            if area < 1000 or area > 20000:
              continue

            # Draw the contours on image
            cv2.drawContours(image, contour[i]+ np.array([x, y]), -1, (255,255,0), 3)

            # cv.minAreaRect returns:
            # (center(x, y), (width, height), angle of rotation) = cv2.minAreaRect(c)
            rect = cv2.minAreaRect(c)
            box = cv2.boxPoints(rect)
            box = np.int0(box)
          
            # Retrieve the key parameters of the rotated bounding box
            center = (int(rect[0][0]),int(rect[0][1])) 
            width = int(rect[1][0])
            height = int(rect[1][1])
            angle = int(rect[2])
          
              
            if width < height:
              angle = 90 - angle
            else:
              angle = -angle
          
            contours.append(contour[0] + np.array([x, y]))  # Shift contour points back to the original image coordinates
            angles.append(angle)

            # Display orientation
            label = "Angle: " + str(angle)
            cv2.putText(oriented_image, label, (center[0]-10, center[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,0,255), 1, cv2.LINE_AA)
            cv2.drawContours(image,[box+ np.array([x, y]) ],0,(0,0,255),2)
        

      # Draw keypoints and edges on input image
      image = utils.visualize(image, detection_result)
      
      # Stop the program if the ESC key is pressed.
      if cv2.waitKey(1) == 27:
        break
      cv2.imshow('object_detector', image)
      cv2.imshow('HSV', hsv)
      cv2.imshow('Gray', gray)

      # Zip the two data arrays together into one iterable
      zip_index_angle_data = zip(class_index,angles)

      # Use list comprehension to flatten the iterable into a list of integers
      flat_index_angle_data = [num for pair in zip_index_angle_data for num in pair]

      # Convert the list of integers into a comma-separated string
      index_angle_data = ','.join(map(str, flat_index_angle_data))

      # Send serial data to Teensy
      if class_index: 
          # Create the serial data string
          serial_data = "1,{},{}".format(len(class_index), index_angle_data)
          print(serial_data)
          # Send the serial data
          ser.write(serial_data.encode())

  # When the camera is unreachable, send alert code 0 -> cameraIsOn = False
  ser.write(b'0')  
  cv2.destroyAllWindows()



def main():
  parser = argparse.ArgumentParser(
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument(
      '--model',
      help='Path of the object detection model.',
      required=False,
      default='improv_4k_02EfficientDet.tflite')
  parser.add_argument(
      '--cameraId', help='Id of camera.', required=False, type=int, default=0)
  parser.add_argument(
      '--frameWidth',
      help='Width of frame to capture from camera.',
      required=False,
      type=int,
      default=640)
  parser.add_argument(
      '--frameHeight',
      help='Height of frame to capture from camera.',
      required=False,
      type=int,
      default=480)
  parser.add_argument(
      '--numThreads',
      help='Number of CPU threads to run the model.',
      required=False,
      type=int,
      default=4)
  parser.add_argument(
      '--enableEdgeTPU',
      help='Whether to run the model on EdgeTPU.',
      action='store_true',
      required=False,
      default=False)
  args = parser.parse_args()

  run(args.model, int(args.cameraId), args.frameWidth, args.frameHeight,
      int(args.numThreads), bool(args.enableEdgeTPU))


if __name__ == '__main__':
  main()
