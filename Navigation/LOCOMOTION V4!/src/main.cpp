#include <Arduino.h>
#include <Navigation.h>
#include <TeensyThreads.h>

int State = 0;
char MegaState = 0;
DriverObject Driver;
USBSerialMaster RaspberryPi; // Change SerialState to TRANSMITTING and put message in buffer
// to send a message to the pi
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
void Thread1(void)
{ // Serial Process Thread

  RaspberryPi.SerialProcess();
  Serial.print("test");
};
void Thread2(void)
{ // Navigation Thread
  // NavStateMachine();
  Serial.print("t2");
};
void setup()
{
  RaspberryPi = USBSerialMaster();
  // threads.addThread(Thread1); // Add thread that processes serial inputs
  // threads.addThread(Thread2); // Add thread that controls navigation
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Starting up");
  State = 1;
};

void loop()
{
  RaspberryPi.SerialProcess();
  // NOTHING IN MY SERIAL LOOP; ALL CODE THAT IS PROCESSED
  // IS IN THE THREADS
};
