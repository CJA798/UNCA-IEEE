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
 
#include "macros.h"
#include "DataPackage.h"

const int pinLED1 = LED1_PIN;

DataPackage dataPackage;

void setup() {
  pinMode(pinLED1, OUTPUT);      // Set pin 13 to output
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() >= 0)) {
    uint8_t input_array[sizeof(DataPackage)];
    size_t input_size = Serial.readBytes(input_array, sizeof(DataPackage));
    dataPackage.parse_data_package(input_array, input_size);
  }
  /*
  if (Serial.available() > 0) {
    
    String data = Serial.readStringUntil('\n');
    Serial.println(data[0]);
     
    if (data == "1") digitalWrite(pinLED1, HIGH);
    else if (data == "0") digitalWrite(pinLED1, LOW);
  }*/
}
