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
  Serial.println("START");
};

void loop()
{
  while (Serial.available())
  {
    if (Serial.read() == '1')
    {
      // Driver.TestMotors();

      Driver.ComputeTranslation(50, 0);
     // Driver.ComputeTranslation(-75, 0);
    };
    Driver.Cleanup();
  };
};
