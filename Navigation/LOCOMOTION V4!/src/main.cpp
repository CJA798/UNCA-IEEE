#include <Arduino.h>
#include <Navigation.h>
#include <Sensors.h>
#include <Drum.h>
DriverObject Driver;
BumperSwitches Bumpers;
TOFArray DistanceSensorsmm;
Drum StorageDrum;
int State = 0;
char InputBit = 0;
void NavStateMachine(void)
{
  switch (State)
  {
  case 0:

    break;
  case 1:
    break;

  default:
    break;
  };
};
void setup()
{
  Driver.UpdateDesiredPose(0, 0, 20);
  Serial.begin(9600);
  Serial.println("STARTING");
  delay(1000);
  MoveState = TRANSLATING;
};

void loop()
{
//  NavStateMachine();
  Driver.ComputeTranslation();
};
