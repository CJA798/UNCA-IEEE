#include <Arduino.h>
#include <TeensyThreads.h>
#include <Navigation.h>
#include <Servo.h>
// Photoresistor Settings
bool PhotoresistorChange(void);
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
Servo ChipDropper = Servo();
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
  RaspberryPi = USBSerialMaster();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PHOTORESISTOR_PIN, INPUT);
  ChipDropper.attach(CHIP_DROPPER_PIN);
  ChipDropper.write(CHIP_DROP_ZERO);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);

  while (Serial.available() == 0)
  {
    // Drum.StartTune();
  };

  Drum.HomeDrumStepper();
 // debug = true;
  debug = false;

  threads.addThread(SerialCommunicationThread);
  State = 1;
  Command = HOME_DRUM;
};

//////////////////////  Loop  ///////////////////////
void loop()
{
  // PhotoresistorChange();
 // Driver.UpdateDesiredPose(0, 0, 45);
 // Driver.ComputeTranslation(0);
 // Driver.UpdateDesiredPose(DEGREE180, 0, 45);
 // Driver.ComputeTranslation(1);
   Drum.DrumProcess();
 // Driver.SweepTheBoard();
  // TestServoDropperStates();
  //  SpinMoveTest();
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
{ /*This state machine will:
 -Start from the origin and first travel towards the first chip drop location; stopping
 before the intake mechanism hits the wall. It will then wait on a message that collection is done,
  or their is nothing to collect, from the raspberry pi.
  -The bot will then move towards the middle of the board about 1/3rd of the way from the top to bottom. (0, 30)
  -The bot will then make another pass towards the west wall of the board again, collecting as it goes; and waiting on an OK from the PI to keep proceeding.
  - bot will then travel to a safe location to rotate without hitting the wall.
  - bot will then rotate 90 degrees to face forward
  - The bot will then move towards the north wall sweeping everything from it to the chip drop location (may require a loop)
  - The bot will then move towards the
 */
  switch (SweepState)
  {
  case 0: // Collect on the way to chip drop one
    Driver.UpdateDesiredPose(0, -95, 2);
    Driver.ComputeTranslation(0);
    SweepState++;
    break;                              // Rotate
  case 1:                               // Move to a safe rotation location
    Driver.UpdateDesiredPose(0, 0, 30); //
    Driver.ComputeTranslation(0);
    SweepState++;
    break;
  case 2:                                      // Rotate 90 deg WE ARE NOW FACING FORWARDS
    Driver.UpdateDesiredPose(DEGREE90, 0, 30); //
    Driver.ComputeTranslation(1);
    SweepState++;
    break;
  case 3:                                                                // Move to chip drop one
    Driver.UpdateDesiredPose(DEGREE90, CHIP_DROP_X, BOTTOM_CHIP_DROP_Y); //
    Driver.ComputeTranslation(0);
    delay(BOT_STOP_DELAY);
    digitalWrite(LED_BUILTIN, HIGH); // DROP CHIP
    ChipDropper.write(CHIP_DROP_ZERO - DROP_DIST);
    delay(SERVO_DROP_DELAY);
    ChipDropper.write(CHIP_DROP_ZERO);
    SweepState++;
    break;
  case 4:
    Driver.UpdateDesiredPose(DEGREE90, CHIP_DROP_X, 75); //
    Driver.ComputeTranslation(0);
    SweepState++;
    break;
  case 5:
    Driver.UpdateDesiredPose(DEGREE90, 0, 45);
    Driver.ComputeTranslation(0);
  }

  Driver.UpdateDesiredPose(0, SOUTH_WALL, 45);
  Driver.ComputeTranslation(0); // rotate 180 degrees clockwise to make the intake system face the switch
  Driver.UpdateDesiredPose(DEGREE180, SOUTH_WALL, 45);
  Driver.ComputeTranslation(1);
  Driver.UpdateDesiredPose(DEGREE180, CHIP_DROP_X, BOTTOM_CHIP_DROP_Y);
  Driver.ComputeTranslation(0);

  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);
  // -<<< ADD DROP CHIP CODE HERE
  ChipDropper.write(CHIP_DROP_ZERO - DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  ChipDropper.write(CHIP_DROP_ZERO);
  State++;
  Driver.UpdateDesiredPose(DEGREE180, CHIP_DROP_X, TOP_CHIP_DROP_Y); // Goto second drop location
  Driver.ComputeTranslation(0);
  delay(BOT_STOP_DELAY);
  digitalWrite(LED_BUILTIN, HIGH);
  // -<<< ADD DROP CHIP CODE HERE
  ChipDropper.write(CHIP_DROP_ZERO + DROP_DIST);
  delay(SERVO_DROP_DELAY);
  digitalWrite(LED_BUILTIN, LOW);
  State++; // <-- ADD DROP CHIP CODE HERE
  Driver.SweepTheBoard();
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
  };
};
bool PhotoresistorChange(void)
{
  DerivativeAvg = 0;
  PrevValue = 0;
  int i;
  for (i = 0; i < NUM_SAMPLES; i++)
  {
    PrevValue += analogRead(PHOTORESISTOR_PIN);
  }
  for (i = 0; i < NUM_SAMPLES; i++)
  {
    DerivativeAvg += analogRead(PHOTORESISTOR_PIN);
  }
  DerivativeAvg = PrevValue - DerivativeAvg;
  // Serial.print("\n Derivate: ");
  // Serial.print(DerivativeAvg);
  // Serial.print("\n PrevValue: ");
  // Serial.print(PrevValue);
  delay(500);
  if (fabs(DerivativeAvg) > LED_ON_THRESHOLD)
  {
    return 1;
  }
  else
  {
    delay(500);
    Serial.print(DerivativeAvg);
    return 0;
  };
};
