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
    //  Serial.println("IdleState");
    //  delay(1000);
    break;
  case 1:
    Driver.UpdateDesiredPose(0, 0, 20);
    MoveState = TRANSLATING;
    State = 2;
    break;
  case 2:
    if (!Driver.IsMoving())
    {
      delay(1000);
      Driver.UpdateDesiredPose(0, 0, 0);
      MoveState = TRANSLATING;
      State = 3;
    }
    break;
  default:
    break;
  };
};
void setup()
{

  Serial.begin(9600);

  while (!Serial.available())
  {
    delay(100);
  };
  Serial.println("STARTING");
  delay(1000);
};

void loop(){
  Driver.BumperProcess();
  NavStateMachine();
};
