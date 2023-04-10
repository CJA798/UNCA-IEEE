#include <Arduino.h>
#include <TeensyThreads.h>
#include <Navigation.h>

#define ROTATE_90_CW 1
#define ROTATE_180_CW 2
#define ROTATE_90_CCW 3
#define ROTATE_180_CCW 4
#define GOTO_NW_POS 5
#define GOTO_NE_POS 6
#define GOTO_SW_POS 7
#define GOTO_SE_POS 8
#define GOTO_POND_POS 9
void note(int num, long dur);

int NavThreadId = 0;
bool Nudging = false;
volatile char State;
void StartTune(void);
DriverObject Driver;
USBSerialMaster RaspberryPi; // Change SerialState to TRANSMITTING and put message in buffer
void NavStateMachine(void);
//////////////////////  Threads  ///////////////////////
void Raspi(void)
{
  while (1)
  {
    RaspberryPi.SerialProcess();
  };
}

//////////////////////  Setup  ///////////////////////
void setup()
{
  RaspberryPi = USBSerialMaster();
  Drum.StartTune();
  debug = false;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Starting up");
  State = 1;
 // debug = true;
  threads.addThread(Raspi);
  Drum.HomeDrumStepper();
};
char temp = 1;
//////////////////////  Loop  ///////////////////////
void loop()
{
  Drum.DrumProcess();
  //NavStateMachine();
 /// Bumpers.SwitchesProcess();
 // Driver.BumperProcess();
};

void NavStateMachine(void)

{
  Serial.print("INSIDE NAV STATE MACHINE: ");

  switch (State)
  {
  case 0:
    Serial.println("IdleState");

    break;
  case 1:
    Driver.UpdateDesiredPose(0, 0, 4);
    MoveState = TRANSLATING;
    State = 2;
    Driver.ComputeTranslation();
    break;
  case 2:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, 0, -2);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();

      State = 3;
    };
    break;
  case 3:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, EAST_WALL, -2);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();

      State = 4;
    };
    break;
  case 4:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, EAST_WALL - 10, NORTH_WALL);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();

      State = 5;
    };
  case 5:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, EAST_WALL, NORTH_WALL);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();

      State = 6;
    };
    break;
  case 6:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, WEST_WALL, NORTH_WALL);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();

      State = 7;
    };
    break;
  case 7:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, WEST_WALL, SOUTH_WALL);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();
      State = 8;
    };
    break;
  case 8:
    if (!Driver.IsMoving())
    {
      delay(100);
      Driver.UpdateDesiredPose(0, 0, NORTH_WALL / 2);
      MoveState = TRANSLATING;
      Driver.ComputeTranslation();
      State = 9;
    };
    break;
  };
};

