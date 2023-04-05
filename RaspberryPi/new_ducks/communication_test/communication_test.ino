/* TODO: 
 *  0) Figure out data transmission from Pi to Teensy
 *  1) For objects inside platforms:
 *      - Control LEDs based on classes
 *      - Control servo based on angle
 *  2) For nearby objects:
 *      - Control LEDs based on number of objects
 *  3) Error codes:
 *      - Signal when camera is not detected
 */
 
#include <Servo.h>
#include <vector>
#include <algorithm>
#include "macros.h"
#include "DataPackage.h"
#include "FlipperPlatform.h"
#include "StorageDrum.h"

const int pinLED1 = BUILT_IN_LED_PIN;
const int pinPinkDuck = PINK_DUCK_PIN;
const int pinYellowDuck = YELLOW_DUCK_PIN;
const int pinRedPillar = RED_PILLAR_PIN;
const int pinGreenPillar = GREEN_PILLAR_PIN;
const int pinWhitePillar = WHITE_PILLAR_PIN;

const int rotationServoPin = ROTATION_SERVO_PIN;
const int flipServoPin = FLIP_SERVO_PIN;
const int clearServoPin = CLEAR_SERVO_PIN;


Servo orientationServo, rotationServo, flipServo, clearServo;
DataPackage dataPackage;
FlipperPlatform flipper(rotationServoPin, flipServoPin, clearServoPin);
//StorageDrum drum;

void setup() {
     
  pinMode(pinLED1, OUTPUT);      // Built-in LED
  pinMode(pinPinkDuck, OUTPUT);
  pinMode(pinYellowDuck, OUTPUT);
  pinMode(pinRedPillar, OUTPUT);
  pinMode(pinGreenPillar, OUTPUT);
  pinMode(pinWhitePillar, OUTPUT);

  orientationServo.attach(ORIENTATION_SERVO_PIN);
  Serial.begin(115200);
  
  waitForPi();
  
}

void loop() {
  // Check if there's a data package
  if (Serial.available() > NO_INCOMING_DATA) {
    // Read string
    String input_string = Serial.readStringUntil('\n');
    if (input_string == "EXIT") {
      clearSignals();  // call clearSignals() function if exit message received
    }
    
    else if (input_string == "RESET") {
      clearSignals();  // call clearSignals() function if exit message received
      waitForPi();
    } 

    else {
      size_t input_size = input_string.length();
      uint8_t* input_array = (uint8_t*) input_string.c_str();
      
      // Parse data from string        
      dataPackage.parse_data_package(input_array, input_size);

      // Turn on LED corresponding to the object class
      digitalWrite(pinPinkDuck, std::find(dataPackage.classObjectsInPlatform.begin(), dataPackage.classObjectsInPlatform.end(), PINK_DUCK) != dataPackage.classObjectsInPlatform.end());
      digitalWrite(pinYellowDuck, std::find(dataPackage.classObjectsInPlatform.begin(), dataPackage.classObjectsInPlatform.end(), YELLOW_DUCK) != dataPackage.classObjectsInPlatform.end());
      digitalWrite(pinRedPillar, std::find(dataPackage.classObjectsInPlatform.begin(), dataPackage.classObjectsInPlatform.end(), RED_PILLAR) != dataPackage.classObjectsInPlatform.end());
      digitalWrite(pinGreenPillar, std::find(dataPackage.classObjectsInPlatform.begin(), dataPackage.classObjectsInPlatform.end(), GREEN_PILLAR) != dataPackage.classObjectsInPlatform.end());
      digitalWrite(pinWhitePillar, std::find(dataPackage.classObjectsInPlatform.begin(), dataPackage.classObjectsInPlatform.end(), WHITE_PILLAR) != dataPackage.classObjectsInPlatform.end());

      // Move servo to correct angle
      flipper.currentAngle = abs(dataPackage.orientationObjectsInPlatform[0]);
      if(flipper.currentAngle > 10){
        flipper.rotatePlatform();
        Serial.println(flipper.currentAngle);
      } else{
        flipper.stopRotation();
        flipper.flipPlatform();
        Serial.println("Stopped");
      }
    }
    dataPackage.print_data_package();    
  }
}

void clearSignals(){
  digitalWrite(pinLED1, LOW);
  digitalWrite(pinYellowDuck, LOW);
  digitalWrite(pinRedPillar, LOW);
  digitalWrite(pinGreenPillar, LOW);
  digitalWrite(pinWhitePillar, LOW);
}

void waitForPi(){
  bool raspi_ready = false;
  while (!raspi_ready) {
    if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      if (message == "ready") {
        raspi_ready = true;
        Serial.println("ready");
      }
    }
  }
}