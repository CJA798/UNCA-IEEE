#include <Arduino.h>






void setup() {
    Serial.begin(9600); // According to the Teensy Docs, this is the RX1 & TX1 on my board.
    // Serial itself corrosponds to the micro-usb port
}
String msg = "Start up";      

void loop() {
       // Serial.availableForWrite();
        Serial.println(msg);   
        delay(1000);
    /*
    if(Serial.available() > 0) {
      msg = "";
      

      
      while(Serial.available() > 0) {
          char read = Serial.read();
          msg += read;
      }
      
      Serial.write('X'); // Acknowledge with reply
    }
    Serial.println(msg); // Output to console for debugging
    // Should be a number 1-9
    // TODO: further processing
    */

}