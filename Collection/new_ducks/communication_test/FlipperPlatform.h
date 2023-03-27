////////////////////////////////////////////////////////////////////////////////
//  Description: This file contains the FlipperPlatform class
//
//  Modified: Carlos Anzola (27 Mar 2023)
////////////////////////////////////////////////////////////////////////////////
//  Ideally, the rotation servo ratates until the object is oriented.
//  Then, the flip servo flips the platform, sending the object to the
//  elevator platform.
//  If there is more than one object, the clean servo clears the platform.
////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>

enum class Flipper_Platform_Error {
  NO_OBJECTS_IN_PLATFORM,
  UNORIENTED_OBJECT,
  ORIENTED_OBJECT,
  FLIPPING_OBJECT,
  FLIPPER_PLATFORM_ERROR_MORE_THAN_ONE_OBJECT,
  FLIPPER_PLATFORM_ERROR_UNKNOWN
};


class FlipperPlatform {
  private:
    // Private attributes
    Servo rotationServo;
    Servo flipServo;
    Servo clearServo;

    // Private helper function to wait for a certain amount of time
    void wait(unsigned long duration) {
      unsigned long startTime = millis();
      while (millis() - startTime < duration) {
        // Do nothing while waiting
      }
    }


  public:
    // Public attributes
    State currentState;
    int currentAngle;
    int numObjects;

  // Constructor to initialize the class attributes    
    FlipperPlatform(int rotationServoPin, int flipServoPin, int clearServoPin) {
      currentState = NO_OBJECTS;
      currentAngle = 0;
      numObjects = 0;      
      rotationServo.attach(rotationServoPin);
      flipServo.attach(flipServoPin);
      clearServo.attach(clearServoPin);
    }

    void rotatePlatform() {
      rotationServo.writeMicroseconds(1400);
    }

    void stopRotation(){
      rotationServo.writeMicroseconds(1500);      
    }

    void flipPlatform() {
      currentState = FLIPPING_PLATFORM
      flipServo.write(80);
      wait(500);
      flipServo.write(10);
      wait(500);
    }

    void clearPlatform() {
      clearServo.write(80);
      wait(500);
      clearServo.write(10);
      wait(500);
    }

    void getCurrentState(Flipper_Platform_Error state) {
      return currentState;
    }
    
    void setCurrentState(Flipper_Platform_Error state) {
      currentState = state;
    }

    int getCurrentAngle() {
      return currentAngle;
    }

    void setCurrentAngle(int angle) {
      currentAngle = angle;
    }

    int getNumObjects() {
      return numObjects;
    }

    void setNumObjects(int num) {
      numObjects = num;
    }

};
