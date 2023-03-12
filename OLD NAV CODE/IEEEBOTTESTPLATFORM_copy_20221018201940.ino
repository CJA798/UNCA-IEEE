/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.
   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.
   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#include <AccelStepper.h>
#include <MultiStepper.h>



#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
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
// Function

void InitMotors(void);

// StateMachine
unsigned int StateMachineCount = 0;
bool OneTime = 0;
//const int BLUETOOTH_TX = 8;
//const int BLUETOOTH_RX = 7;
int incDist = 1;
int SpinSpeed = 400;
int Radius;
float angle;
bool onetime = 0;

// Motor Assignments

//SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
//ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor.

/*
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

    // AStepper.setSpeed(testspeed);
    //DStepper.setSpeed(testspeed);
    //CStepper.setSpeed(testspeed);
    //BStepper.setSpeed(testspeed);
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


*/
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
 // if (StateMachineCount >= 10) {
 //   StateMachineCount = 0;
    Dabble.processInput();  //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
 // }
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