#include <main.h>
DriverObject Driver;

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

void setup()
{

  Serial.begin(9600);

  delay(5000);
  Serial.println("START");
}

void loop()
{
  Driver.SwitchesProcess();
  Driver.ComputeMoveAbs(Origin);

  Driver.ComputeMoveAbs(One);

  Driver.ComputeRotation(SpinRt);
  Driver.ComputeRotation(SpinRt);
  delay(7000);
}
