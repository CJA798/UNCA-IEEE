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

void setup()
{
  // Wire2.begin();
  Serial.begin(115200);
  delay(5000);
  Serial.println("START");
};

void loop()
{
  switch(NavState){
case 0:
  break;
  case 1:
// STARTING COLLECTION LOOP



  break;
case 2:
// COLLECTING ITEMS
  break;
case 3:
  break;
};
};

void I2CScannerProcess(void)
{

  // set the 24C256 eeprom address to 0
  Wire2.beginTransmission(0x50);
  if (!Wire2.available())
  {
    Serial.println("no response");
  };

  // Wire2.write(0); // address high byte
  // Wire2.write(0); // address low byte
  // Wire2.endTransmission();

  delay(5000);
};
