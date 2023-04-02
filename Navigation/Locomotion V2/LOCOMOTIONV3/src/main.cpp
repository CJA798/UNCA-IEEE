#include <main.h>
DriverObject Driver;
BumperSwitches Bumpers;

// #include <Wire.h>

// Points of interest for testing
double Origin[3][1] = {{0}, {0}, {50}};
double One[3][1] = {{0}, {0}, {-50}};
double Two[3][1] = {{0}, {25}, {0}};
double Three[3][1] = {{0}, {25}, {25}};
double Four[3][1] = {{0}, {-50}, {-50}};

double SpinRt[3][1] = {{TWO_PI}, {0}, {0}};
double SpinLft[3][1] = {{-TWO_PI}, {0}, {0}};

double HalfSpinRt[3][1] = {{PI}, {0}, {0}};
double HalfSpinLft[3][1] = {{-PI}, {0}, {0}};
int State = 1;
int FirstTime = 1;
char InputBit = 0;
byte num = 0;
void I2CScannerProcess(void);
// void NavigationStateMachine(void);

void setup()
{
 // Wire2.begin();
 Serial.begin(115200);
 delay(5000);
 Serial.println("START");
}

void loop()
{
/*
  if(Serial.available()){
    InputBit = Serial.read();
  }
  */
 Driver.NewMoveQueue.AddMove(0, 10, 0);
 delay(10000);
 // Bumpers.SwitchesProcess();
  Driver.NavigationProcess();
 // NavigationStateMachine();
 // I2CScannerProcess();

  delay(1000);
}

void NavigationStateMachine()
{
  switch (State)
  {
  case 1:

    break;
  case 2:
    if (Driver.IsRunning() > 0)
    {
      State = 3;
    };
    break;
  case 3:
    Driver.NavState = 1;
    //  Driver.InputPose[0][0] = PI / 2;
    State = 4;
    break;
  case 4:
    if (Driver.IsRunning() > 0)
    {
      State = 5;
    };
    break;
  case 5:

    State = 6;
    break;
  case 6:

    break;
  };
};

void I2CScannerProcess(void)
{

  // set the 24C256 eeprom address to 0
Wire2.beginTransmission(0x50);
if (!Wire2.available() ){
  Serial.println("no response");
};

 // Wire2.write(0); // address high byte
  //Wire2.write(0); // address low byte
 // Wire2.endTransmission();


  delay(5000);
};
