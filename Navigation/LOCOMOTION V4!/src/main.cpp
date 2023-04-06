#include <main.h>
DriverObject Driver;
BumperSwitches Bumpers;

// #include <Wire.h>

int State = 1;
char InputBit = 0;

void setup()
{
  // Wire2.begin();
  Serial.begin(115200);

  Driver.UpdateDesiredPose(PI / 2, 25, 25);
  ComplexMoveState = 1;
  Serial.println("START");  
  delay(6000);
};

void loop()
{

  Driver.ComplexMove();
  delay(100);
};
