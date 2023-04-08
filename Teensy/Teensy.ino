 #include <Servo.h>
 #include "PusherINO_max.h"

 uint16_t IncomingByte;
 #define StandByCase  (0b0000'0000'0000'0000)
 //Back braces for tower building
 #define BotBrace  (0b0000'0000'0000'0001)
 #define SwingBrace  (0b0000'0000'0000'0010)
 #define TopBrace  (0b0000'0000'0000'0100)
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //Pushers for loading and unloading the drum
 #define BotPusherLoading  (0b0000'0000'0000'1000)
 #define BotPusherHalf_Unloading  (0b0000'0000'0001'0000)
 #define BotPusherUnloading  (0b0000'0000'0001'1000)
 
 #define TopPusherLoading  (0b0000'0000'0000'1000)
 #define TopPusherHalf_Unloading  (0b0000'0000'0001'0000)
 #define TopPusherUnloading  (0b0000'0000'0001'1000)
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 #define ElevatorPlateOn  (0b0000'0000'0010'0000)
 #define ElevatorRaise  (0b0000'0000'0100'0000)

 #define SweepEject  (0b0000'0010'0000'0000)
 #define SweepPush  (0b0000'0100'0000'0000)

 #define FlipperPlateOn  (0b0000'1000'0000'0000)
 #define FlipperFlip  (0b0001'0000'0000'0000)

 #define IntakeOn  (0b0010'0000'0000'0000)
 #define IntakeOpp  (0b0100'0000'0000'0000)
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 #define CameraFailure  (0b1000'0000'0000'0000)

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //Servo Code for setting them up

Servo IntakeRight;
Servo IntakeLeft;
Servo IntakeLoad;
Servo Flipper;
Servo FlipperPlate;
Servo Sweeper;
Servo Elevator;
Servo ElevatorPlate;
Servo PusherTop;
Servo PusherBot;
Servo BraceTop;
Servo SwingerBrace;
Servo BraceBot;

PusherINO Pushers;

#define ContOn      (180)
#define ContOff     (90)
#define SERVO1_PIN  (1)
#define SERVO2_PIN  (2)
#define SERVO3_PIN  (3)
#define SERVO4_PIN  (4)
#define SERVO5_PIN  (5)
#define SERVO6_PIN  (6)
#define SERVO7_PIN  (7)
#define SERVO8_PIN  (8)
#define SERVO9_PIN  (9)
#define SERVO10_PIN  (10)
#define SERVO11_PIN  (11)
#define SERVO12_PIN  (12)
#define SERVO13_PIN  (24) 

void setup() {
  Serial.begin(115200);
  while(!Serial){}

  IntakeRight.attach(SERVO1_PIN);
  IntakeLeft.attach(SERVO2_PIN);
  IntakeLoad.attach(SERVO3_PIN);
  Flipper.attach(SERVO4_PIN);
  FlipperPlate.attach(SERVO5_PIN);
  Sweeper.attach(SERVO6_PIN);
  Elevator.attach(SERVO7_PIN);
  ElevatorPlate.attach(SERVO8_PIN);
  //PusherTop.attach(SERVO9_PIN);
  //PusherBot.attach(SERVO10_PIN);
  BraceTop.attach(SERVO11_PIN);
  SwingerBrace.attach(SERVO12_PIN);
  BraceBot.attach(SERVO13_PIN);

  IntakeRight.write(90);
  IntakeLeft.write(90);
  IntakeLoad.write(90);
  Flipper.write(0);
  FlipperPlate.write(90);
  Sweeper.write(90);
  Elevator.write(0);
  ElevatorPlate.write(90);
  PusherTop.write(0);
  PusherBot.write(0);
  BraceTop.write(0);
  SwingerBrace.write(0);
  BraceBot.write(0);
  IncomingByte = 0;

}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n'); // Read the input string
    int intValue = inputString.toInt(); // Convert the input string to an integer
    uint16_t IncomingByte = intValue & 0xFFFF; // Mask out all but the lowest 16 bits
    Serial.println(IncomingByte, BIN);
    
  }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Bracing if statements. When they are 1 then the brace needs to activate, otherwise it will go back to state 0
  if(IncomingByte & BotBrace){

  } else{

  }
  if(IncomingByte & SwingBrace){

  } else{
    
  }
  if(IncomingByte & TopBrace){

  } else{
    
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The pusher has 4 different states. The first one will be retracted, then loading, half unloading for the columns and finally fully unloaded.
  //00 retracted
  //01 Loading
  //10 Half Unloading
  //11 Unloading
  if(IncomingByte & BotPusherLoading){
    Serial.println("BotPusher: UNLOADING");
    // TODO: run UNLOADING sequence
  } else if(IncomingByte & BotPusherHalf_Unloading){
    Serial.println("BotPusher: HALF-UNLOADING");
    // TODO: run UNLOADING sequence
  } else if(IncomingByte & BotPusherUnloading){
    Serial.println("BotPusher: UNLOADING");
    // TODO: run UNLOADING sequence
  } else{
    Serial.println("BotPusher: RETRACTING");
    // TODO: run UNLOADING sequence
  }

  if(IncomingByte & TopPusherLoading){
    Serial.println("TopPusher: UNLOADING");
    PusherTop.write(10);
  } else if(IncomingByte & TopPusherHalf_Unloading){
    Serial.println("TopPusher: HALF-UNLOADING");
    PusherTop.write(20);
  } else if(IncomingByte & TopPusherUnloading){
    Serial.println("TopPusher: UNLOADING");
    PusherTop.write(30);
  } else{
    Serial.println("TopPusher: RETRACTING");
    PusherTop.write(0);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The elevator is another case of being completely on or off.
  if(IncomingByte & ElevatorPlateOn){
    Serial.println("ElevatorPlate: ON");
    EevatorPlatel.writeMicroseconds(1700);
  } else{
    Serial.println("ElevatorPlate: OFF");
    ElevatorPlate.writeMicroseconds(1500);
  }

  if(IncomingByte & ElevatorRaise){
    Serial.println("ElevatorRaise: ON");
    Elevator.write(90);
  } else{
    Serial.println("ElevatorRaise: OFF");
    Elevator.write(0);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The sweeper has three states
  //00 Resting
  //01 Ejecting Item
  //10 Pushing item onto elevator
  if(IncomingByte & SweepEject){
    Serial.println("SweepEject: ON");
    Sweeper.write(10);
  } else if(IncomingByte & SweepPush){
    Serial.println("SweepPush: ON");
    Sweeper.write(20);
  } else{
    Serial.println("Sweep: OFF");
    Sweeper.write(0);
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The flipper has two states for each of it's points
  if(IncomingByte & FlipperPlateOn){
    Serial.println("FlipperPlate: ON");
    FlipperPlate.writeMicroseconds(1700);
  } else{
    Serial.println("FlipperPlate: OFF");
    FlipperPlate.writeMicroseconds(1500);
  }

  if(IncomingByte & FlipperFlip){
    Serial.println("FlipperFlip: ON");
    Flipper.write(20);
  } else{
    Serial.println("FlipperPlate: OFF");
    Flipper.write(0);
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The intake has 3 states
  //00 Off
  //01 On
  //10 Opposite spinning for Jammed
  if(IncomingByte & IntakeOn){
    Serial.println("Intake: ON");
    IntakeRight.write(0);
    IntakeLeft.write(180);
    IntakeLoad.write(180);
  } else if(IncomingByte & IntakeOpp){
    Serial.println("Intake: OPPOSITE ON");
    IntakeRight.write(180);
    IntakeLeft.write(180);
    IntakeLoad.write(0);
  } else{
    Serial.println("Intake: OFF");
    IntakeRight.write(90);
    IntakeLeft.write(90);
    IntakeLoad.write(90);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The camera failed
  if(!(IncomingByte & CameraFailure)){
    Serial.println("Camera: OFF");
  }

  Serial.flush();
  IncomingByte = 0;
}
