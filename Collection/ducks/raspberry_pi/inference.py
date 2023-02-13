"""Main script to run the object detection routine."""
import argparse
import sys
import time

import cv2
import utils

from math import atan2, cos, sin, sqrt, pi
import numpy as np


from roboflow import Roboflow
rf = Roboflow(api_key="bgDsByRHahH20OhzbvRB")
project = rf.workspace("ieee-computer-vision").project("ieee-dataset-v3")
model = project.version(2, local="http://localhost:9001/").model



def run(camera_id: int, width: int, height: int) -> None:
  """Continuously run inference on images acquired from the camera.

  Args:
    model: Name of the TFLite object detection model.
    camera_id: The camera id to be passed to OpenCV.
    width: The width of the frame captured from the camera.
    height: The height of the frame captured from the camera.
  """

  # Variables to calculate FPS
  counter, fps = 0, 0
  start_time = time.time()

  # Start capturing video input from the camera
  cap = cv2.VideoCapture(camera_id)
  cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
  cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

  # Visualization parameters
  row_size = 20  # pixels
  left_margin = 24  # pixels
  text_color = (0, 0, 255)  # red
  font_size = 1
  font_thickness = 1
  fps_avg_frame_count = 10

  # Continuously capture images from the camera and run inference
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      sys.exit(
          'ERROR: Unable to read from webcam. Please verify your webcam settings.'
      )

    # Duplicate image. First one, for object detection; second one, for object orientation.
    oriented_image = image

    # Convert oriented_image to grayscale
    gray = cv2.cvtColor(oriented_image, cv2.COLOR_BGR2GRAY)

    # Convert oriented_image to binary
    _, bw = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

    # Find all the contours in the thresholded oriented_image
    contours, _ = cv2.findContours(bw, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
    #create an empty image for contours
    img_contours = np.zeros(oriented_image.shape)
    # draw the contours on the empty image
    cv2.drawContours(img_contours, contours, -1, (0,255,0), 3)
    #cv2.imshow('contours', img_contours)
    
    # Enumerate contours
    for i, c in enumerate(contours):
      # Calculate the area of each contour
      area = cv2.contourArea(c)
      #cv2.imshow('segmentation', area)
      
      # Ignore contours that are too small or too large
      if area < 10000 or area > 120000:
        continue
    
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

      # Display orientation
      label = "  Angle: " + str(angle) + " degrees"
      textbox = cv2.rectangle(oriented_image, (center[0]-35, center[1]-25), (center[0] + 295, center[1] + 10), (255,255,255), -1)
      cv2.putText(oriented_image, label, (center[0]-50, center[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,0,0), 1, cv2.LINE_AA)
      cv2.drawContours(oriented_image,[box],0,(0,0,255),2)

    counter += 1
    image = cv2.flip(image, 1)

    # Convert the image from BGR to RGB as required by the TFLite model.
    rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Run object detection estimation using the model.
    prediction = model.predict(rgb_image, confidence=50, overlap=30)
    print(prediction.json())

    # Draw keypoints and edges on input image
    #image = utils.visualize(image, prediction)

    # Calculate the FPS
    if counter % fps_avg_frame_count == 0:
      end_time = time.time()
      fps = fps_avg_frame_count / (end_time - start_time)
      start_time = time.time()

    # Show the FPS
    fps_text = 'FPS = {:.1f}'.format(fps)
    text_location = (left_margin, row_size)
    cv2.putText(image, fps_text, text_location, cv2.FONT_HERSHEY_PLAIN,
                font_size, text_color, font_thickness)

    # Stop the program if the ESC key is pressed.
    if cv2.waitKey(1) == 27:
      break
    cv2.imshow('object_detector', image)
    cv2.imshow('contours', img_contours)
    

  cap.release()
  cv2.destroyAllWindows()


def main():
  parser = argparse.ArgumentParser(
      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
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
  args = parser.parse_args()

  run(int(args.cameraId), args.frameWidth, args.frameHeight)


if __name__ == '__main__':
  main()
