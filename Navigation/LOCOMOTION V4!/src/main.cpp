bool debug;
// flags
int Command;
#include <Arduino.h>
#include <TeensyThreads.h>
#include <Navigation.h>
#include <Servo.h>
#include <Wire.h>
// #include <Photoresistor.h>
//  Photoresistor Settings
void Home(void);

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// stores the red, green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

bool redFlag = 0;
bool greenFlag = 0;
bool blueFlag = 0;
bool whiteFlag = 0;
bool FirstTime = true;
bool PhotoresistorChange(void);
float PhotoresistSum1;
float PhotoresistSum2;
int DerivativeRollingAverage[20] = {0};
int DerivativeWriteCnt = 0;
int DerivativeAvg = 0;
int PrevValue = 0;
#define LED_ON_THRESHOLD 25
void TestServoDropperStates(void);
int NavThreadId = 0;
bool Nudging = false;
char SweepState = 0;
volatile char State;
Servo ChipDropper;
DriverObject Driver;
// USBSerialMaster RaspberryPi; // Change SerialState to TRANSMITTING and put message in buffer
void NavStateMachine(void);
void SpinMoveTest(void);
void LEDsetup(void);
void LEDloop(void);
//////////////////////  Threads  ///////////////////////
void SerialCommunicationThread(void)
{
  while (true)
  {
    Drum.StartTune();
  };
};
//////////////////////  Setup  ///////////////////////
void setup()
{
  analogReadRes(16);
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  ChipDropper.attach(CHIP_DROPPER_PIN);
  ChipDropper.write(CHIP_DROP_ZERO);
  threads.addThread(SerialCommunicationThread);
  State = 1;
  LEDsetup();
  //  Command = HOME_DRUM;

  while (!PhotoresistorChange())
  {
    delay(10);
  };
};

//////////////////////  Loop  ///////////////////////
void loop()
{

  Serial.print("made it past photoresist");
  // TESTING -----------------
  // PhotoresistorChange();
  // Driver.UpdateDesiredPose(0, 0, 45);
  // Driver.ComputeTranslation(0);
  // Driver.UpdateDesiredPose(DEGREE180, 0, 45);
  // Driver.ComputeTranslation(1);
  // Driver.SweepTheBoard();
  // TestServoDropperStates();
  //  SpinMoveTest();
  // MAIN LOOP -----------------
  Driver.UpdateDesiredPose(0, 0, 4); // MOVE FORWARD
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, WEST_WALL, 0); // GO TO CHIP DROP ONE
  Driver.ComputeTranslation(0);
  ChipDropper.write(CHIP_DROP_ZERO - DROP_DIST); // DROP CHIPS
  Serial.print("made it past chip dropper\n");
  delay(700);
  Driver.UpdateDesiredPose(0, WEST_WALL, NORTH_WALL); // GO TO CHIP DROP TWO
  Driver.ComputeTranslation(0);
  ChipDropper.write(CHIP_DROP_ZERO + DROP_DIST); // DROP CHIPS
  delay(700);

  Driver.UpdateDesiredPose(0, WEST_WALL + 10, 10);
  Driver.ComputeTranslation(0);
  Home();
  Driver.UpdateDesiredPose(0, EAST_WALL, NORTH_WALL);
  Driver.ComputeTranslation(0);
  Home();
  Driver.UpdateDesiredPose(0, EAST_WALL, SOUTH_WALL);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, EAST_WALL, NORTH_WALL);
  Driver.ComputeTranslation(0);
  Home();
  Driver.UpdateDesiredPose(0, WEST_WALL, 10);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, WEST_WALL + 10, NORTH_WALL / 2);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, EAST_WALL + 10, NORTH_WALL - 5);
  Driver.ComputeTranslation(0);
  Home();
  Driver.UpdateDesiredPose(0, EAST_WALL, SOUTH_WALL);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, EAST_WALL, NORTH_WALL + 10);
  Driver.UpdateDesiredPose(0, WEST_WALL, BotOrientation.Y);
  Driver.ComputeTranslation(0);
  Home();
  Driver.UpdateDesiredPose(0, 0, NORTH_WALL);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, EAST_WALL, SOUTH_WALL);
  Driver.ComputeTranslation(0);
  while (true)
  {
    Driver.UpdateDesiredPose(0, EAST_WALL - 20, NORTH_WALL - 10);
    Driver.ComputeTranslation(0);

    Driver.UpdateDesiredPose(0, EAST_WALL + 10, SOUTH_WALL);
    Driver.ComputeTranslation(0);
    Driver.UpdateDesiredPose(0, EAST_WALL + 2, NORTH_WALL + 1);
    BotOrientation.X -= 10;
  };
}; // DO THE SWEEP

void Home(void)
{
  Driver.UpdateDesiredPose(0, 0, 45);
  Driver.ComputeTranslation(0);
  Driver.UpdateDesiredPose(0, 0, 0);
  Driver.ComputeTranslation(0);
};
void SpinMoveTest(void)
{
  Driver.UpdateDesiredPose(BotOrientation.Theta, 0, 45);
  Driver.UpdateDesiredPose(BotOrientation.Theta + DEGREE90, 0, 45);
}

/*

y Ctrl+H
Compute Translation
Bot Pose
Theta: 0.00
X: 0.00
Y: 0.00
Input Pose
Theta: (should be zero )0.00
X: 0.00
Y: 45.00

 DID NOT USE MATRIX TRANSFORMATION
0.0000
0.0000
45.0000
NewWheelSteps:
2309.3999
-2309.3999
2309.3999
-2309.3999
Message: w
Command: ␀
Compute Translation
Bot Pose
Theta: 0.00
X: 0.00
Y: 45.00
Input Pose
Theta: (should be zero )3.14
X: 0.00
Y: 45.00

 DID NOT USE MATRIX TRANSFORMATION
3.1416
0.0000
0.0000

 Rotational Move
NewWheelSteps:
-4131.1934
-4131.1934
-4131.1934
-4131.1934



3.1445.000.00


Compute Translation
Bot Pose
Theta: 3.14
X: 0.00
Y: 45.00
Input Pose
Theta: (should be zero )3.14
X: -95.00
Y: 45.00

DOING MATRIX TRANSFORMS

TranslationalMatrix1
1.00000.00000.0000
0.00001.00000.0000
0.000045.00001.0000
TranslationalMatrix2
1.00000.00000.0000
0.00001.00000.0000
0.0000-45.00001.0000
HomogoenousMatrix
-95.00000.0000
0.000045.0000
0.00001.0000
RotationalMatrix
-1.0000-0.00000.0000
0.0000-1.00000.0000
0.00000.00001.0000
T1
-1.0000-0.00000.0000
0.0000-1.00000.0000
0.0000-45.00001.0000
T2
-1.0000-0.00000.0000
0.0000-1.00000.0000
0.0000-90.00001.0000
TransformedMatrix
95.0000-0.0000
-0.0001-45.0000
-0.0027-4049.0000
0.0000
95.0000
-0.0000
NewWheelSteps:
4875.3984
4875.4014
-4875.4014
-4875.3984
Compute Translation
Bot Pose
Theta: 3.14
X: 95.00
Y: 45.00
Input Pose
Theta: (should be zero )3.14
X: -114.00
Y: 55.00

DOING MATRIX TRANSFORMS

TranslationalMatrix1
1.00000.00000.0000
0.00001.00000.0000
95.000045.00001.0000
TranslationalMatrix2
1.00000.00000.0000
0.00001.00000.0000
-95.0000-45.00001.0000
HomogoenousMatrix
-209.000010.0000
95.000045.0000
0.00001.0000
RotationalMatrix
-1.0000-0.00000.0000
0.0000-1.00000.0000
0.00000.00001.0000
T1
-1.0000-0.00000.0000
0.0000-1.00000.0000
-95.0000-45.00001.0000
T2
-1.0000-0.00000.0000
0.0000-1.00000.0000
-190.0000-90.00001.0000
TransformedMatrix
208.9999-10.0001
-95.0001-45.0000
31159.9922-5949.0029
0.0000
208.9999
-10.0001
NewWheelSteps:
10212.6738
11239.0801
-11239.0801
-10212.6738
Compute Translation
Bot Pose
Theta: 3.14
X: 304.00
Y: 35.00
Input Pose
Theta: (should be zero )3.14
X: -95.00
Y: 65.00

DOING MATRIX TRANSFORMS

TranslationalMatrix1
1.00000.00000.0000
0.00001.00000.0000
303.999934.99991.0000
TranslationalMatrix2
1.00000.00000.0000
0.00001.00000.0000
-303.9999-34.99991.0000
HomogoenousMatrix
-398.999930.0001
303.999934.9999
0.00001.0000
RotationalMatrix
-1.0000-0.00000.0000
0.0000-1.00000.0000
0.00000.00001.0000
T1
-1.0000-0.00000.0000
0.0000-1.00000.0000
-303.9999-35.00011.0000
T2
-1.0000-0.00000.0000
0.0000-1.00000.0000
-607.9999-70.00001.0000
TransformedMatrix
398.9998-30.0001
-304.0002-34.9999
221311.9219-20689.0430
0.0000
398.9998
-30.0001
NewWheelSteps:
18937.0625
22016.2734
-22016.2734
-18937.0625

            ____________________________________________
            | ChipDropX, btdropY                       |
            |   ^                                      |
            |   |                                      |
            |   |                                      |
            |   |                                      |
            |   |                                      |
            |   |                                      |
            |   |                                      |
            | ChipDropX, BtmDrpY                       |
            |__________________________________________|
*/

void TestServoDropperStates(void)
{

  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);
  // -<<< ADD DROP CHIP CODE HERE
  ChipDropper.write(CHIP_DROP_ZERO - DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  ChipDropper.write(CHIP_DROP_ZERO);
  delay(4000);
  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);
  // -<<< ADD DROP CHIP CODE HERE
  ChipDropper.write(CHIP_DROP_ZERO + DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  State = 1; // <-- ADD DROP CHIP CODE HERE
  delay(5000);
};
void SweepBoardStateMachine(void)
{ /* State Machine List of Coordinates. This state machine will not

*/

  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);

  ChipDropper.write(CHIP_DROP_ZERO - DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  ChipDropper.write(CHIP_DROP_ZERO);
  Driver.UpdateDesiredPose(DEGREE180, CHIP_DROP_X, TOP_CHIP_DROP_Y); // Goto second drop location
  Driver.ComputeTranslation(0);
  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);

  ChipDropper.write(CHIP_DROP_ZERO + DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
};

void NavStateMachine(void)

{
  switch (State)
  {
  case 0: // WAITING FOR PHOTORESISTOR TO BE TRIGGERED TO START MOVEMENT
    if (PhotoresistorChange() == true)
    {
      Driver.UpdateDesiredPose(0, 0, 3);
      State = 1;
      Driver.ComputeTranslation(0);
    }
    break;
  case 1:
    Driver.UpdateDesiredPose(0, CHIP_DROP_X, BOTTOM_CHIP_DROP_Y);
    State = 2;
    Driver.ComputeTranslation(0);
    break;
  case 2:

    delay(100);
    Driver.UpdateDesiredPose(0, 0, -2);
    Driver.ComputeTranslation(0);

    State = 3;
    break;
  case 3:

    delay(100);
    Driver.UpdateDesiredPose(0, EAST_WALL, -2);
    Driver.ComputeTranslation(0);

    State = 4;
    break;
  case 4:
    delay(100);
    Driver.UpdateDesiredPose(0, EAST_WALL - 10, NORTH_WALL);
    Driver.ComputeTranslation(0);

    State = 5;
  case 5:

    delay(100);
    Driver.UpdateDesiredPose(0, EAST_WALL, NORTH_WALL);
    Driver.ComputeTranslation(0);

    State = 6;

    break;
  case 6:
    delay(100);
    Driver.UpdateDesiredPose(0, WEST_WALL, NORTH_WALL);
    Driver.ComputeTranslation(0);
    State = 7;

    break;
  case 7:

    delay(100);
    Driver.UpdateDesiredPose(0, WEST_WALL, SOUTH_WALL);
    Driver.ComputeTranslation(0);
    State = 8;

    break;
  case 8:

    delay(100);
    Driver.UpdateDesiredPose(0, 0, NORTH_WALL / 2);
    Driver.ComputeTranslation(0);
    State = 9;
    break;

  case LAST_CASE:
    Command = 0;
    break;
  };
};

bool PhotoresistorChange(void)
{

  if (FirstTime == true) // take an average of 10 readings if it is the first time we are running this
  {

    for (int i = 0; i < 10; i++)
    {
      Serial.print("\n\n FirstTime \n\n");
      PhotoresistSum1 += analogRead(PHOTORESISTOR_PIN);
      delay(1);

      Serial.println(PhotoresistSum1);
    };
    PhotoresistSum1 = PhotoresistSum1 / 10;
  };
  FirstTime = false;
  // Serial.print("\n\n FirstTime \n\n");
  Serial.println(PhotoresistSum1);
  PhotoresistSum2 = analogRead(PHOTORESISTOR_PIN);
  if (PhotoresistSum2 < (PhotoresistSum1 + THRESHOLD))
  {
    FirstTime = false;
    return false;
  }
  else
  {

    Serial.print("==========================================");

    FirstTime = false;
    return true;
  };
};

void LEDsetup(void)
{
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Begins serial communication
  // Serial.begin(9600);
};
void LEDloop(void)
{
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // resetting flags
  redFlag = 0;
  greenFlag = 0;

  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // redColor = map(redFrequency, 70, 120, 255,0);    //get low and max values by holding sensor close/far from color
  redColor = map(redFrequency, 30, 80, 255, 0);

  // Printing the RED (R) value
  Serial.print("R = ");
  // Serial.print(redFrequency); // replace with redColor once determined
  Serial.print(redColor);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, 80, 170, 255, 0);

  // Printing the GREEN (G) value
  Serial.print(" G = ");
  // Serial.print(greenFrequency); // change to greenColor
  Serial.print(greenColor);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, 60, 190, 255, 0);

  // Printing the BLUE (B) value
  Serial.print(" B = ");
  // Serial.println(blueFrequency);  //change to blueColor
  Serial.println(blueColor);
  delay(100);

  // Checks the current detected color and prints
  // a message in the serial monitor

  if (redColor > greenColor && redColor > blueColor)
  {
    Serial.println(" - RED detected!");
    redFlag = 1;
  }
  if (greenColor > redColor && greenColor > blueColor)
  {
    Serial.println(" - GREEN detected!");
    greenFlag = 1;
  }
};