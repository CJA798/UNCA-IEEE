#include <Arduino.h>
#include <Navigation.h>
#include <TeensyThreads.h>
#define ROTATE_90_CW 1
#define ROTATE_180_CW 2
#define ROTATE_90_CCW 3
#define ROTATE_180_CCW 4
#define GOTO_NW_POS 5
#define GOTO_NE_POS 6
#define GOTO_SW_POS 7
#define GOTO_SE_POS 8
#define GOTO_POND_POS 9
int State = 0;
char MegaState = 0;
bool NavigationProcessRunning = false;
DriverObject Driver;
USBSerialMaster RaspberryPi; // Change SerialState to TRANSMITTING and put message in buffer

//////////////////////  Threads  ///////////////////////
void NavigationThread(void)
{
  while (NavigationProcessRunning)
  {
    Driver.BumperProcess();
  };
}

//////////////////////  Setup  ///////////////////////
void setup()
{
  RaspberryPi = USBSerialMaster();
  debug = false;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Starting up");
  State = 1;
 // debug = true;
  threads.addThread(NavigationThread);
  Drum.HomeDrumStepper();
};
//////////////////////  Loop  ///////////////////////
void loop()
{
  RaspberryPi.SerialProcess();
  Drum.DrumProcess();
};
void RaspberryPiAsMaster()
{
  switch (Command)
  {
  };
}
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
