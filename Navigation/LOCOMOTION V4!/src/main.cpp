#include <Arduino.h>
#include <Navigation.h>
#include <Drum.h>
DriverObject Driver;

Drum StorageDrum;
int State = 0;
char InputBit = 0;
void NavStateMachine(void)
{
  switch (State)
  {
  case 0:
      Serial.println("IdleState");
      delay(1000);
    break;
  case 1:
    Driver.UpdateDesiredPose(0, 0, 10);
    MoveState = TRANSLATING;
    State = 2;
    Driver.ComputeTranslation();
    break;
  case 2:
    if (!Driver.IsMoving())
    {
      delay(1000);
      Driver.UpdateDesiredPose(0, 0, -20);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();
      State = 3;
    };
    break;
  case 3:
    if (!Driver.IsMoving())
    {
    Driver.ComputeTranslation();
    Driver.UpdateDesiredPose(0, EAST_WALL, 0);
    MoveState = TRANSLATING;
    Driver.ComputeTranslation();
    State = 4;
    };
  default:
  case 4:
    if (!Driver.IsMoving())
    {
      delay(1000);
      Driver.UpdateDesiredPose(0, 0, 0);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();
      State = 5;
    };
    break;
  };
};
void setup()
{

  Serial.begin(9600);

 // while (!Serial.available())
  //{
    delay(100);
 // };
//  State = 1;
  Serial.println("STARTING");
 // delay(1000);
};

void loop(){
StorageDrum.TestRotation();
 // Driver.BumperProcess();
 // NavStateMachine();
};
