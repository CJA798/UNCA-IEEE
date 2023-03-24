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

/*
bool itemOnFlipper = false;
bool itemOnElevator = false;
bool itemFlipped = false;
bool itemAlignedElevator = false;
bool itemAlignedFlipper = false;
bool elevatorHeightReached = false;
*/

Servo flipperCont;
/*
Servo flipIt;
Servo elevatorCont;
Servo elevatorHeight;
Servo pushToElevator;
Servo pushToCylinder;
*/

DataPackage dataPackage;

void setup() {
     
  pinMode(pinLED1, OUTPUT);      // Set pin 13 to output
  pinMode(pinPinkDuck, OUTPUT);
  pinMode(pinYellowDuck, OUTPUT);
  pinMode(pinRedPillar, OUTPUT);
  pinMode(pinGreenPillar, OUTPUT);
  pinMode(pinWhitePillar, OUTPUT);

  flipperCont.attach(ORIENTATION_SERVO_PIN);
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
      int angleFlip = dataPackage.orientationObjectsInPlatform[0];    
      /*
      int angleElevator = Your elevator Angle
      int upperBound = 10;
      int lowerBound = 200;
      int intoCylinder = 10;
      int outOfCylinder = 200;
      int upperHeight = 90;
      int lowerHeight = 10;
      if((itemOnElevator == false)&&(itemOnFlipper == true)){
        if(itemAlignedFlipper == true){
          //flipIt()
          flipIt.write(upperbound);
          delay(400);
          flipIt.write(lowerbound);
          itemAlignedFlipper = false;
          itemOnFlipper = false;
        }
        else{
          if((abs(angleFlip) < 10)||(abs(angleFlip) > 20)){
            flipperCont.write(60);        
            delay(100);
            flipperCont.write(90);         
          }
          else{
            itemAlignedFlipper = true;
          }
        }
      }
      else if((itemOnElevator == true)&&(itemOnFlipper == true)){
        if(itemAlignedElevator == true){
          //elevateIt()
          if(elevatorHeightReached == true){
            //pushIt()
            pushToCylinder.write(intoCylinder);
            delay(400);
            pushToCylinder.write(outOfCylinder);
            delay(400);
            elevatorHeight.write(lowerHeight);
            elevatorHeightReached = false;
            itemAlignedElevator = false;
            itemOnElevator = false;
          }
          else{
            elevatorHeight.write(upperHeight);
            delay(400);
            elevatorHeightReached = true;
          }
        }
        else{
          if((abs(angleElevator) < 10)||(abs(angleElevator) > 20)){
            elevatorCont.write(60);        
            delay(100);
            elevatorCont.write(90);         
          }
          else{
            itemAlignedElevator = true;
          }
        }
      }
      */
      if((abs(angleFlip) < 10)||(abs(angleFlip) > 20)){
        flipperCont.write(60);        
        delay(100);
        flipperCont.write(90);         
      }
      else{
        flipperCont.write(90);        
      }
    }
    dataPackage.print_data_package();    
  }
  //dataPackage.print_data_package();
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