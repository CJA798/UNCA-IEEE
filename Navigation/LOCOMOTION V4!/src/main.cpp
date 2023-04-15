#include <Arduino.h>
#include <TeensyThreads.h>
#include <Navigation.h>
#include <Servo.h>
// #include <Photoresistor.h>
//  Photoresistor Settings

bool PhotoresistorChange(void);
int PhotoresistSum1;
int PhotoresistSum2;
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
USBSerialMaster RaspberryPi; // Change SerialState to TRANSMITTING and put message in buffer
void NavStateMachine(void);
void SpinMoveTest(void);
//////////////////////  Threads  ///////////////////////
void SerialCommunicationThread(void)
{
  while (true)
  {
    RaspberryPi.SerialProcess();
  };
};
char temp = 1;
//////////////////////  Setup  ///////////////////////
void setup()
{

  bool FirstTime = true;
  pinMode(PHOTORESISTOR_PIN, INPUT);
  RaspberryPi = USBSerialMaster();
  pinMode(LED_BUILTIN, OUTPUT);
  ChipDropper.attach(CHIP_DROPPER_PIN);
  ChipDropper.write(CHIP_DROP_ZERO);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
  while (Serial.available() == 0)
  {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    // Drum.StartTune();
  };

  // Drum.HomeDrumStepper();
  // debug = true;
  // debug = false;

  threads.addThread(SerialCommunicationThread);
  State = 1;
  //  Command = HOME_DRUM;
};

//////////////////////  Loop  ///////////////////////
void loop()
{
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
  Drum.DrumProcess(); // Function immediately returns if DrumState is not set too NEW_MOVE.
  if (Command > 0)
  {
    if (debug)
    {
      Serial.println("Command");
      Serial.print(Command);
    };

    switch (Command)
    {
    case InYellowDuck1:
      if (debug)
      {
        Serial.println("InYellowDuck1");
      };
      NewPosition = IN_POSITION_YELLOW_DUCK1;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case InYellowDuck2:
      NewPosition = IN_POSITION_YELLOW_DUCK2;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case InPinkDuck:
      NewPosition = IN_POSITION_PINK_DUCK;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case InWhiteColumn:
      NewPosition = IN_POSITION_WHITE_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case InGreenColumn:
      NewPosition = IN_POSITION_GREEN_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case InRedColumn:
      NewPosition = IN_POSITION_RED_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutYellowDuck1:
      NewPosition = OUT_POSITION_YELOW_DUCK2;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutYellowDuck2:
      NewPosition = OUT_POSITION_YELLOW_DUCK1;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutPinkDuck:
      NewPosition = OUT_POSITION_PINK_DUCK;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutWhiteColumn:
      NewPosition = OUT_POSITION_WHITE_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutGreenColumn:
      NewPosition = OUT_POSITION_GREEN_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
    case OutRedColumn:
      NewPosition = OUT_POSITION_RED_PILLAR;
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      break;
      // case HOME_DRUM: We dont need this state, homing on startup.
      // HomeDrumStepper();
      // break;
    case SPECIAL:
      Serial.print("finished");
      Serial.println(Command);
      Command = 0;
      Drum.StartTune();
      Drum.HomeDrumStepper();
      break;
    case RUN_STATE_MACHINE: // Returns once our move is complete.
      NavStateMachine();    // Will keep on progressing through NavStateMachine each
      break;                // time the loop is ran untill the NavStateMachine is finished. To interrupt the SM send in a new command.
    default:
      break;
    };
  };
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

  if (FirstTime) // take an average of 10 readings if it is the first time we are running this
  {
    for (int i = 0; i < 10; i++)
    {
      PhotoresistSum1 += analogRead(PHOTORESISTOR_PIN);
      delay(1);
    };
  }
  else
  {
    for (int i = 0; i < 10; i++)
    { // take an average of 10 readings (for comparison) if we are are not running this for the first time.
      PhotoresistSum2 += analogRead(PHOTORESISTOR_PIN);
      delay(1);
    };
  };
  FirstTime = false;
  PhotoresistSum2 = 0;
  if (PhotoresistSum2 > PhotoresistSum1 + LED_ON_THRESHOLD)
  {
    return true;
  }
  else
  {
    return false;
  };
};
