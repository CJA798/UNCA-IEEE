#include <Servo.h> 
#include <Arduino.h> 
 
Servo servo;
const int servoPin = 20;
const int dataPin = 7;
 
void setup() 
{ 
  servo.attach(servoPin);  // attaches the servo on pin 20
  pinMode(dataPin, INPUT);    // sets the digital pin 7 as input

} 
 
 
void loop() 
{
  bool state = digitalRead(dataPin);
  Serial.println(state);

} 