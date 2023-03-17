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

const int pinLED1 = BUILT_IN_LED_PIN;
const int pinPinkDuck = PINK_DUCK_PIN;
const int pinYellowDuck = YELLOW_DUCK_PIN;
const int pinRedPillar = RED_PILLAR_PIN;
const int pinGreenPillar = GREEN_PILLAR_PIN;
const int pinWhitePillar = WHITE_PILLAR_PIN;

Servo orientationServo;

DataPackage dataPackage;

void setup() {
  pinMode(pinLED1, OUTPUT);      // Set pin 13 to output
  pinMode(pinPinkDuck, OUTPUT);
  pinMode(pinYellowDuck, OUTPUT);
  pinMode(pinRedPillar, OUTPUT);
  pinMode(pinGreenPillar, OUTPUT);
  pinMode(pinWhitePillar, OUTPUT);

  orientationServo.attach(ORIENTATION_SERVO_PIN);
  
  Serial.begin(9600);
}

void loop() {
  // Check if there's a data package
  if (Serial.available() > NO_INCOMING_DATA) {
    // Read string
    String input_string = Serial.readStringUntil('\n');
    if (input_string == "EXIT") {
      clearSignals();  // call clearSignals() function if exit message received
    } else {
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
      int angle = dataPackage.orientationObjectsInPlatform[0];    
      orientationServo.write(abs(angle));
      Serial.println(angle);
    }
  }
}

void clearSignals(){
  digitalWrite(pinLED1, LOW);
  digitalWrite(pinYellowDuck, LOW);
  digitalWrite(pinRedPillar, LOW);
  digitalWrite(pinGreenPillar, LOW);
  digitalWrite(pinWhitePillar, LOW);
}
