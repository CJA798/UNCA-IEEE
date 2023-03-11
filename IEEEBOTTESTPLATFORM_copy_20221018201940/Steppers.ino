
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

void Backward(int distance, int speed);
void InitMotors(void);
void KeepTime(void);


AccelStepper AStepper(motorInterfaceType, AStep, ADir);
AccelStepper BStepper(motorInterfaceType, BStep, BDir);
AccelStepper CStepper(motorInterfaceType, CStep, CDir);
AccelStepper DStepper(motorInterfaceType, DStep, DDir);
MultiStepper steppers;


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
