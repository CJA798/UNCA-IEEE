/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.
   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.
   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Dabble.h>

#define SDPOWER -1

//ChipSelect, Hardware SS Pin on Mega, 10 for Arduino Boards, always kept as output
#define SDCS_PIN 53
#define SD_DETECT_PIN -1  //currently not implemented

#define LED_PIN 13

#define FAN_PIN 9

#define PS_ON_PIN 12  //ATX , awake=LOW, SLEEP=High
#define KILL_PIN -1

#define HEATER_0_PIN 10  // Extruder Heater
#define HEATER_1_PIN 8

#define TEMP_0_PIN 13  // ANALOG NUMBERING
#define TEMP_1_PIN 14  // ANALOG NUMBERING
#define motorInterfaceType 1

/* Motor Definitions: Motors are labeled right to left looking at the board with the USB port at the top right;
i.e                                 arrangements on bot
  --------                    ============
  | A  B |                    |A        B|
  | C  D |                    |          |
  | E    |                    |          |
  --------                    |C        D|
                              ============
  (original schematic)

  --------
  | X  E0 |
  | Y  E1 |
  | Z     |
  --------
*/

// MOTOR A (X)
const int AStep = 54;
const int ADir = 55;
const int AEnable = 38;
const int ACS = 53;

// MOTOR C (Y)
const int CStep = 60;
const int CDir = 61;
const int CEnable = 56;
const int CCS = 49;

// MOTOR E (Z)
const int EStep = 46;
const int EDir = 48;
const int EEnable = 62;
const int ECS = 40;

// MOTOR B (E0)
const int BStep = 26;
const int BDir = 28;
const int BEnable = 24;
const int BCS = 42;

// MOTOR D (E1)
const int DStep = 36;
const int DDir = 34;
const int DEnable = 30;
const int DCS = 44;

// StateMachine
unsigned int StateMachineCount = 0;
bool OneTime = 0;
void Backward(int distance, int speed);
int incDist = 1;
int SpinSpeed = 400;

int Radius;
float angle;
bool onetime = 0;
void InitMotors(void);

// Motor Assignments
AccelStepper AStepper(motorInterfaceType, AStep, ADir);
AccelStepper BStepper(motorInterfaceType, BStep, BDir);
AccelStepper CStepper(motorInterfaceType, CStep, CDir);
AccelStepper DStepper(motorInterfaceType, DStep, DDir);
MultiStepper steppers;

//SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
//ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor.
void testSpin() {
  int pos1 = 500;
  int pos2 = 0;
  int testspeed = 200;
  int testpos = 400;
  if (GamePad.isStartPressed()) {
    onetime = 1;
  }
  if (GamePad.isSelectPressed()) {
    onetime = 0;
  }
  if (onetime) {
    if (AStepper.distanceToGo() == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      AStepper.moveTo(-AStepper.currentPosition());
      BStepper.moveTo(-BStepper.currentPosition());
      CStepper.moveTo(-CStepper.currentPosition());
      DStepper.moveTo(-DStepper.currentPosition());
    }

    AStepper.run();
    BStepper.run();
    CStepper.run();
    DStepper.run();
  }
}

void testMotors() {
  CStepper.setSpeed(200);
  CStepper.move(10);
  CStepper.run();
}

void SpinLft() {
  AStepper.setSpeed(-SpinSpeed);
  DStepper.setSpeed(-SpinSpeed);
  CStepper.setSpeed(-SpinSpeed);
  BStepper.setSpeed(-SpinSpeed);

  AStepper.move(incDist);
  BStepper.move(incDist);
  CStepper.move(incDist);
  DStepper.move(incDist);
  AStepper.run();
  BStepper.run();
  CStepper.run();
  DStepper.run();
}


void SpinRt() {
  AStepper.setSpeed(SpinSpeed);
  DStepper.setSpeed(SpinSpeed);
  CStepper.setSpeed(SpinSpeed);
  BStepper.setSpeed(SpinSpeed);

  AStepper.move(incDist);
  BStepper.move(incDist);
  CStepper.move(incDist);
  DStepper.move(incDist);
  AStepper.run();
  BStepper.run();
  CStepper.run();
  DStepper.run();
}

void forWard(){
  AStepper.setSpeed(SpinSpeed);
  CStepper.setSpeed(SpinSpeed);
  DStepper.setSpeed(-SpinSpeed);
  BStepper.setSpeed(-SpinSpeed);


  AStepper.move(incDist);
  BStepper.move(incDist);
  CStepper.move(incDist);
  DStepper.move(incDist);
  AStepper.run();
  BStepper.run();
  CStepper.run();
  DStepper.run();
}

void BackWard(){
  AStepper.setSpeed(-SpinSpeed);
  CStepper.setSpeed(-SpinSpeed);
  DStepper.setSpeed(SpinSpeed);
  BStepper.setSpeed(SpinSpeed);


  AStepper.move(incDist);
  BStepper.move(incDist);
  CStepper.move(incDist);
  DStepper.move(incDist);
  AStepper.run();
  BStepper.run();
  CStepper.run();
  DStepper.run();
}

// This function initializes the motor pins
void InitMotors(void) {

  AStepper.setMaxSpeed(2000);
  AStepper.setAcceleration(10);
  AStepper.setSpeed(1000);


  BStepper.setMaxSpeed(2000);
  BStepper.setAcceleration(10);
  BStepper.setSpeed(1000);


  CStepper.setMaxSpeed(2000);
  CStepper.setAcceleration(10);
  CStepper.setSpeed(1000);


  DStepper.setMaxSpeed(2000);
  DStepper.setAcceleration(10);
  DStepper.setSpeed(1000);



  pinMode(AStep, OUTPUT);
  pinMode(ADir, OUTPUT);
  pinMode(AEnable, OUTPUT);
  pinMode(ACS, OUTPUT);

  pinMode(BStep, OUTPUT);
  pinMode(BDir, OUTPUT);
  pinMode(BEnable, OUTPUT);
  pinMode(BCS, OUTPUT);

  pinMode(CStep, OUTPUT);
  pinMode(CDir, OUTPUT);
  pinMode(CEnable, OUTPUT);
  pinMode(CCS, OUTPUT);

  pinMode(DStep, OUTPUT);
  pinMode(DDir, OUTPUT);
  pinMode(DEnable, OUTPUT);
  pinMode(DCS, OUTPUT);

  pinMode(EStep, OUTPUT);
  pinMode(EDir, OUTPUT);
  pinMode(EEnable, OUTPUT);
  pinMode(ECS, OUTPUT);
  return;
}

void move(void) {
  if (GamePad.isCirclePressed()) {

    digitalWrite(LED_BUILTIN, HIGH);
    SpinRt();
    //forWard();
  }
  if (GamePad.isSquarePressed()) {

    digitalWrite(LED_BUILTIN, HIGH);
    SpinLft();
  }
  if (GamePad.isTrianglePressed()) {

    digitalWrite(LED_BUILTIN, HIGH);
    forWard();
  }
  if (GamePad.isCrossPressed()) {

    digitalWrite(LED_BUILTIN, HIGH);
    BackWard();
  }
  if (Radius > 1) {
    Serial.print("angle = ");
    Serial.println(angle);
    int speedA = (int)(Radius * cos(angle - (PI / 4)));
    int speedB = (int)(Radius * cos(angle + (PI / 4)));
    int speedC = (int)(Radius * sin(angle - (PI / 4)));
    int speedD = (int)(-Radius * cos(angle - (PI / 4)));
    Serial.println("A,B,C,D speeds =");
    Serial.println(speedA);
    Serial.println(speedB);
    Serial.println(speedC);
    Serial.println(speedD);

    AStepper.setSpeed(speedA);
    BStepper.setSpeed(speedB);
    CStepper.setSpeed(speedC);
    DStepper.setSpeed(speedD);

    AStepper.move(incDist);
    BStepper.move(incDist);
    CStepper.move(incDist);
    DStepper.move(incDist);
    AStepper.run();
    BStepper.run();
    CStepper.run();
    DStepper.run();
  }
}

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  InitMotors();

  Serial.begin(230400);  // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);    //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
  digitalWrite(LED_BUILTIN, LOW);

  //testMotors();
}

void loop() {
    Dabble.processInput();  //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  angle = GamePad.getAngle();
  Radius = GamePad.getRadius();
  Radius = map(Radius, 0, 7, 0, 400);
  angle = (angle * PI) / 180;
  digitalWrite(LED_BUILTIN, LOW);

  // m,testSpin();
  if (Radius >=1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  move();
}