#include <Arduino.h>
#include <Navigation.h>
#include <stdio.h>
#include <string.h>
using namespace std;
#define RECEIVING 'R'
#define TRANSMITTING 'T'
// define buffer size
const int BUFFER_SIZE = 20;

// define buffer and index variable
char buffer[BUFFER_SIZE];
int indx = 0;
bool InputReady = 0;
char InputChar = 0;
char SerialState = 0;
class USBSerial
{
public:
  int SerialWriteCounter = 0;
  USBSerial()
  {
    Serial.begin(9600);
  };
  void SerialProcess(void)
  {
    switch (SerialState)
    {
    case RECEIVING:

      if (Serial.available() > 0)
      {
        //  Serial.print("frig");
        //  delay(1000);

        InputChar = Serial.read();
        buffer[indx] = InputChar;
        indx++;
        while (InputChar != '|')
        {
          InputChar = Serial.read();

          buffer[indx] = InputChar;
          indx++;
          if (indx > BUFFER_SIZE - 1)
          {
            indx = 0;
          }

          if (InputChar == '|')
          {
            SerialState = TRANSMITTING;
          };
        };
      }
      break;
    case TRANSMITTING:
      Serial.println("Transmitting");
      for (int i = 0; i < BUFFER_SIZE; i++)
      {
        Serial.print(buffer[i]);
      };
      SerialState = RECEIVING;
      break;
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
  State = 1;
};

void loop()
{
  NavStateMachine();
  // LaptopSerial.SerialProcess();
  DrumState = 0;
  Driver.Process();
  //  item = InYellowDuck2;
  //  Drum.doRotation();
  //  switch (MegaState)
  //  {
  //  case 0: // DO NOTHING
  //  digitalWrite(LED_BUILTIN, HIGH);
  //   break;
  //  }tt
};
