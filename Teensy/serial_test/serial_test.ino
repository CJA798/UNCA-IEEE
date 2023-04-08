 #include <Servo.h>
 uint16_t IncomingByte;
 #define StandByCase  (0b0000'0000'0000'0000)
 //Back braces for tower building
 #define BotBrace  (0b0000'0000'0000'0001)
 #define DuckBrace  (0b0000'0000'0000'0010)
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
  PusherTop.attach(SERVO9_PIN);
  PusherBot.attach(SERVO10_PIN);
  BraceTop.attach(SERVO11_PIN);
  SwingerBrace.attach(SERVO12_PIN);
  BraceBot.attach(SERVO13_PIN);

  IntakeRight.write(90);
  IntakeLeft.write(90);
  IntakeLoad.write(90);
  Flipper.write(-40);
  FlipperPlate.writeMicroseconds(1500);
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

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //The flipper has two states for each of it's points
  if(IncomingByte & FlipperPlateOn){
    FlipperPlate.writeMicroseconds(1700);
    Serial.println("Flipper orienting");
  } else{
    FlipperPlate.writeMicroseconds(1500);
    Serial.println("Flipper OFF");
  }
  if(IncomingByte & FlipperFlip){
    Flipper.write(60);
    Serial.println("Flipper flipping");
  } else{
    Flipper.write(0);
    Serial.println("Flipper not flipping");
  }

  Serial.flush();
  IncomingByte = 0;
}
