#include <Arduino.h>
#include <Navigation.h>
#define SERIAL_CHAR_STORAGAE_LENGTH 15
char InputBit[15] = {0};
class USBSerial
{
public:
  int SerialWriteCounter = 0;
  USBSerial()
  {
    Serial.begin(115200);
  };
  void SerialProcess(void)
  {
    if (Serial.available())
    {
      Serial.readStringUntil('\n').toCharArray(InputBit, SERIAL_CHAR_STORAGAE_LENGTH);
      for (int i = 0; i < SERIAL_CHAR_STORAGAE_LENGTH; i++)
      {
        Serial.print(InputBit[i]);
      };
      Serial.println();
    };
  };
};
USBSerial LaptopSerial;
int State = 0;
char MegaState = 0;
DriverObject Driver;
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
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Starting up");
  Drum.HomeDrumStepper();
};

void loop()
{
  DrumState = 0;
  // Driver.Process();
 
  Drum.doRotation(InRedColumn);
  // switch (MegaState)
  // {
  // case 0: // DO NOTHING
  // digitalWrite(LED_BUILTIN, HIGH);
  //  break;
  // }
};
