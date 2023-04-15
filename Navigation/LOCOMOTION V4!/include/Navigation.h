#include <macros.h>
#include <TeensyStep.h>
#include <math.h>

#include <Serial.h>
#include <Drum.h>
#include <Ports.h>

// #include <iostream>
#include <MiscFunctions.h>

bool NavigationProcessRunning = true;
#include <Sensors.h>
using namespace std;

struct Orientation
{
  double Theta; // Difference in angle between the bots current frame of reference
  // and the global frame of reference
  double X; // Position of the bots global frame of reference in the X direction
  double Y; // Position of the bots global frame of reference in the Y direction
};
// PUBLIC VARIABLES FROM NAVIGATION PROCESS
char NavState = IDLE; // A nav state of 0 means the bot is not moving. A nav state of 1 means the bot is
// making a linear move, a Nav state of 2 means the bot is making a rotational move.
bool IsMoving = IDLE;
char MoveState = IDLE;
char ComplexMoveState = IDLE;

bool ComplexMoveStarted = IDLE;

StorageDrum Drum;
Orientation BotOrientation;
BumperSwitches Bumpers;
elapsedMillis MoveTimer;
class DriverObject
{

public:
  //  CONSTRUCTOR FOR THE NAVIGATION PROCESS. THIS IS CALLED WHEN THE NAVIGATION PROCESS IS INITIALIZED.
  DriverObject() : motor_1(MTR1DIR, MTR1STEP), // MUST PASS CONSTRUCTOR OBJECTS FROM THE STEPPER CLASS
                   motor_2(MTR2DIR, MTR2STEP),
                   motor_3(MTR3DIR, MTR3STEP),
                   motor_4(MTR4DIR, MTR4STEP),
                   Controller()

  // MUST PASS CONSTRUCTOR OBJECTS FROM THE CONTROLLER CLASS
  {
    motor_1
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2
    motor_2
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2
    motor_3
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2
    motor_4
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2
    BotOrientation.Theta = 0;
    BotOrientation.X = 0;
    BotOrientation.Y = 0;
    for (int i = 0; i < 3; i++)
    {
      InputPose[i][0] = 0;
    };
  };
  void SetSpeeds(int MaxSpeed, int MaxAccel)
  {
    motor_1
        .setMaxSpeed(MaxSpeed)      // steps/s
        .setAcceleration(MaxAccel); // steps/s^2
    motor_2
        .setMaxSpeed(MaxSpeed)      // steps/s
        .setAcceleration(MaxAccel); // steps/s^2
    motor_3
        .setMaxSpeed(MaxSpeed)      // steps/s
        .setAcceleration(MaxAccel); // steps/s^2
    motor_4
        .setMaxSpeed(MaxSpeed)      // steps/s
        .setAcceleration(MaxAccel); // steps/s^2
  };
  void SweepTheBoard(void)
  {
    // UpdateDesiredPose(BotOrientation.Theta, SWEEP_START_X, SWEEP_START_Y);
    // ComputeTranslation(0);
    Serial.print("\n\n\n");
    Serial.print(BotOrientation.Theta);
    Serial.print(BotOrientation.Y);
    Serial.print(BotOrientation.X);
    Serial.print("\n\n\n");
    int SweepY = BotOrientation.Y;
    for (int i = 0; i < SWEEP_COUNT; i++)
    {
      UpdateDesiredPose(BotOrientation.Theta, -SWEEP_MAX_X, SweepY);
      ComputeTranslation(0);
      SweepY += SWEEP_DISTANCE_INCREMENT;
      UpdateDesiredPose(BotOrientation.Theta, WEST_WALL, SweepY);
      ComputeTranslation(0);
      SweepY += SWEEP_DISTANCE_INCREMENT;
    };
  };
  //
  void BumperProcess(void)
  {

    if (debug)
    {
      Serial.println("Bumper Process Started");
      Serial.print("X: ");
      Serial.print(BotOrientation.X);
      Serial.print(" Y: ");
      Serial.print(BotOrientation.Y);
      Serial.print(" Theta: ");
      Serial.println(BotOrientation.Theta);
      Serial.println();
      Serial.print("Switches State: ");
      Serial.println(SwitchesState);
      Serial.println();
    };
    // NavigationProcessRunning = false;
    Controller.emergencyStop();
    // Nudging = true;
    //  delay(100);
    if ((BotOrientation.Theta <= PI / 3) && (BotOrientation.Theta >= -(PI / 3)))
    { // We are pointed North
      //
      if (debug)
        Serial.println("Facing North");

      switch (SwitchesState)
      {
      case FRONT_PRESSED:
        BotOrientation.Y = NORTH_WALL;
        Serial.println("Facing North, Front Pressed");
        Nudge(NORTH);
        break;
      case LEFT_PRESSED:
        BotOrientation.X = WEST_WALL;
        Serial.println("Facing North, Left Pressed");
        Nudge(WEST);
        break;
      case RIGHT_PRESSED:
        BotOrientation.X = EAST_WALL;
        Serial.println("Facing North, Right Pressed");
        Nudge(EAST);
        break;
      case BACK_PRESSED:
        BotOrientation.Y = SOUTH_WALL;
        Serial.println("Facing North, Back Pressed");
        Nudge(SOUTH);
        break;
      };
      SwitchesState = NONE_PRESSED;

      return;
    };
    if ((BotOrientation.Theta >= PI / 6) && (BotOrientation.Theta <= (11 / 16) * PI))
    { // We are pointed East
      //
      switch (SwitchesState)
      {
      case FRONT_PRESSED:
        BotOrientation.X = EAST_WALL;
        Nudge(EAST);
        break;
      case LEFT_PRESSED:
        BotOrientation.Y = NORTH_WALL;
        Nudge(NORTH);
        break;
      case RIGHT_PRESSED:
        BotOrientation.Y = SOUTH_WALL;
        Nudge(SOUTH);
        break;
      case BACK_PRESSED:
        BotOrientation.X = WEST_WALL;
        Nudge(WEST);
        break;
      };
      SwitchesState = NONE_PRESSED;
      return;
    };
    if ((BotOrientation.Theta <= -PI / 6) && (BotOrientation.Theta >= -(11 / 16) * PI))
    { // We are pointed West
      switch (SwitchesState)
      {
      case FRONT_PRESSED:
        BotOrientation.X = WEST_WALL;
        Nudge(WEST);
        break;
      case LEFT_PRESSED:
        BotOrientation.Y = SOUTH_WALL;
        Nudge(SOUTH);
        break;
      case RIGHT_PRESSED:
        BotOrientation.Y = NORTH_WALL;
        Nudge(NORTH);
        break;
      case BACK_PRESSED:
        BotOrientation.X = EAST_WALL;
        Nudge(EAST);
        break;
      };
      //
      SwitchesState = NONE_PRESSED;
      return;
    };
    if ((BotOrientation.Theta >= (5 / 6) * PI) || (BotOrientation.Theta <= -(5 / 6) * PI))
    { // We are pointed South
      switch (SwitchesState)
      {
      case FRONT_PRESSED:
        BotOrientation.Y = SOUTH_WALL;
        Nudge(SOUTH);
        break;
      case LEFT_PRESSED:
        BotOrientation.X = EAST_WALL;
        Nudge(EAST);
        break;
      case RIGHT_PRESSED:
        BotOrientation.X = WEST_WALL;
        Nudge(WEST);
        break;
      case BACK_PRESSED:
        BotOrientation.Y = NORTH_WALL;
        Nudge(NORTH);
        break;
      };
      SwitchesState = NONE_PRESSED;
      return;

      //
    };
    // SwitchesState = NONE_PRESSED;
    // MoveState = IDLE;
  };
  void Nudge(char NorthSouthOrWest)
  {
    Serial.println("BEFORE NUDGE");
    Serial.print("X: ");
    Serial.print(BotOrientation.X);
    Serial.print(" Y: ");
    Serial.print(BotOrientation.Y);
    Serial.print(" Theta: ");
    Serial.println(BotOrientation.Theta);
    Serial.println(NorthSouthOrWest);
    switch (NorthSouthOrWest)
    {
    case NORTH:

      motor_1.setTargetRel(-NUDGE_DISTANCE);
      motor_2.setTargetRel(NUDGE_DISTANCE);
      motor_3.setTargetRel(-NUDGE_DISTANCE);
      motor_4.setTargetRel(NUDGE_DISTANCE);
      break;
    case SOUTH:
      motor_1.setTargetRel(NUDGE_DISTANCE);
      motor_2.setTargetRel(-NUDGE_DISTANCE);
      motor_3.setTargetRel(NUDGE_DISTANCE);
      motor_4.setTargetRel(-NUDGE_DISTANCE);

      break;
    case WEST:
      motor_1.setTargetRel(NUDGE_DISTANCE);
      motor_2.setTargetRel(NUDGE_DISTANCE);
      motor_3.setTargetRel(-NUDGE_DISTANCE);
      motor_4.setTargetRel(-NUDGE_DISTANCE);
      // UpdateDesiredPose(BotOrientation.Theta, BotOrientation.X + 5, BotOrientation.Y);
      // ComputeTranslation();
      break;
    case EAST:
      motor_1.setTargetRel(-NUDGE_DISTANCE);
      motor_2.setTargetRel(-NUDGE_DISTANCE);
      motor_3.setTargetRel(NUDGE_DISTANCE);
      motor_4.setTargetRel(NUDGE_DISTANCE);
      // UpdateDesiredPose(BotOrientation.Theta, BotOrientation.X - 5, BotOrientation.Y);
      // ComputeTranslation();
      break;
    };
    motor_1
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_2
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_3
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    motor_4
        .setMaxSpeed(NUDGE_SPEED)
        .setAcceleration(NUDGE_ACCEL);
    Controller.move(motor_1, motor_2, motor_3, motor_4);
    delay(5000);
    Serial.println("after NUDGE");
    Serial.print("X: ");
    Serial.print(BotOrientation.X);
    Serial.print(" Y: ");
    Serial.print(BotOrientation.Y);
    Serial.print(" Theta: ");
    Serial.println(BotOrientation.Theta);
    SwitchesState = NONE_PRESSED;
  };
  bool IsMoving(void)
  {
    return Controller.isRunning();
  };
  void UpdateOrientation(double Theta, double X, double Y)
  {
    BotOrientation.Theta = Theta;
    BotOrientation.X = X;
    BotOrientation.Y = Y;
  };
  void Cleanup()
  {
    BotOrientation.Theta = 0;
    BotOrientation.X = 0;
    BotOrientation.Y = 0;
    motor_1.setPosition(0);
    motor_2.setPosition(0);
    motor_3.setPosition(0);
    motor_4.setPosition(0);
  };

  void ComputeTranslation(bool Rotate)
  { // Process that will move the bot from BotOrientation to InputPose[3][1]. MoveState must be TRANSLATING to run.

    // if (Controller.isRunning())
    // {
    //   return;
    // };
    double DeltaTheta = 0;
    if (Rotate == true)
    {
      DeltaTheta = (InputPose[0][0] - BotOrientation.Theta); // Compute the angle to rotate
    };
    double DeltaX = (InputPose[1][0] - BotOrientation.X); // Compute the X and Y distances to move
    double DeltaY = (InputPose[2][0] - BotOrientation.Y);
    if (debug == true)
    {
      Serial.println("Compute Translation");
      Serial.println("Bot Pose");
      Serial.print("Theta: ");
      Serial.println(BotOrientation.Theta);
      Serial.print("X: ");
      Serial.println(BotOrientation.X);
      Serial.print("Y: ");
      Serial.println(BotOrientation.Y);
      Serial.println("Input Pose");
      Serial.print("Theta: (should be zero )");
      Serial.println(InputPose[0][0]);
      Serial.print("X: ");
      Serial.println(InputPose[1][0]);
      Serial.print("Y: ");
      Serial.println(InputPose[2][0]);
    }
    if ((BotOrientation.Theta != 0) && (!Rotate))
    {
      // If the bot is at an ange
      Serial.println("");
      Serial.println("DOING MATRIX TRANSFORMS\n");
      TranslationalMatrix1[2][0] = BotOrientation.X;  // Set X
      TranslationalMatrix1[2][1] = BotOrientation.Y;  // Set Y
      TranslationalMatrix2[2][0] = -BotOrientation.X; // Set X
      TranslationalMatrix2[2][1] = -BotOrientation.Y; // Set Y
      HomogoenousMatrix[0][0] = DeltaX;               // Set X
      HomogoenousMatrix[0][1] = DeltaY;               // Set Y
      HomogoenousMatrix[1][0] = BotOrientation.X;
      HomogoenousMatrix[1][1] = BotOrientation.Y;
      RotationalMatrix[0][0] = cos(BotOrientation.Theta);  // Set cos(Theta)
      RotationalMatrix[0][1] = -sin(BotOrientation.Theta); // Set -sin(Theta)
      RotationalMatrix[1][0] = sin(BotOrientation.Theta);  // Set sin(Theta)
      RotationalMatrix[1][1] = cos(BotOrientation.Theta);  // Set cos(Theta)
      if (debug == true)
      {
        Serial.println("TranslationalMatrix1");
        PrintMatrix((mtx_type *)TranslationalMatrix1, 3, 3);
        Serial.println("TranslationalMatrix2");
        PrintMatrix((mtx_type *)TranslationalMatrix2, 3, 3);
        Serial.println("HomogoenousMatrix");
        PrintMatrix((mtx_type *)HomogoenousMatrix, 3, 2);
        Serial.println("RotationalMatrix");
        PrintMatrix((mtx_type *)RotationalMatrix, 3, 3);
      }
      // compute the matrix multiplications: T1 = Trans1*Rot , T2 = T1*Trans2, TransformedMatrix = T2*HomoMatrix
      MatrixMultiply((mtx_type *)TranslationalMatrix1, (mtx_type *)RotationalMatrix, 3, 3, 3, (mtx_type *)T1);

      MatrixMultiply((mtx_type *)T1, (mtx_type *)TranslationalMatrix2, 3, 3, 3, (mtx_type *)T2);

      MatrixMultiply((mtx_type *)T2, (mtx_type *)HomogoenousMatrix, 3, 3, 2, (mtx_type *)TransformedMatrix);
      if (debug == true)
      {
        Serial.println("T1");
        PrintMatrix((mtx_type *)T1, 3, 3);
        Serial.println("T2");
        PrintMatrix((mtx_type *)T2, 3, 3);
        Serial.println("TransformedMatrix");
        PrintMatrix((mtx_type *)TransformedMatrix, 3, 2);
      }
      UpdateDesiredPose(0, TransformedMatrix[0][0], TransformedMatrix[0][1]); // Update the desired pose
    };
    if (BotOrientation.Theta == 0)
    {                           // If bot is not at an angle
      InputPose[1][0] = DeltaX; // Set the X and Y distances to move
      InputPose[2][0] = DeltaY;
      Serial.println(" ");
      Serial.println(" DID NOT USE MATRIX TRANSFORMATION");
    };
    PrintMatrix((mtx_type *)InputPose, 3, 1);
    if (Rotate)
    {
      Serial.print("\n Rotational Move \n");
      UpdateDesiredPose(DeltaTheta, 0, 0);
    };
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)InputPose, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    UpdateMotorObjects();
    // Update the stepper objects
    BotOrientation.X += InputPose[1][0]; // Update the bots position
    BotOrientation.Y += InputPose[2][0];
    if (Rotate)
    {
      BotOrientation.Theta += DeltaTheta;
      if ((BotOrientation.Theta > PI) || (BotOrientation.Theta < -PI))
      {
        BotOrientation.Theta -= PI;
      };
    };
  };
  void ComputeRotation(void) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  {
    // if (Controller.isRunning())
    //{
    // return;
    //};
    double DeltaTheta = (InputPose[0][0] - BotOrientation.Theta); // Compute the Theta distance to move
    InputPose[0][0] = DeltaTheta;                                 // Set the Theta distance to move
    InputPose[1][0] = 0;
    InputPose[2][0] = 0;
    if (debug == true)
    {
      Serial.println("Rotation");
      Serial.println("Bot Pose");
      Serial.print("Theta: ");
      Serial.println(BotOrientation.Theta);
      Serial.print("X: ");
      Serial.println(BotOrientation.X);
      Serial.print("Y: ");
      Serial.println(BotOrientation.Y);
      Serial.println("Theta: ");
      PrintMatrix((mtx_type *)InputPose, 3, 1);
    };
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)InputPose, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    UpdateMotorObjects();                                                                                   // Update the stepper objects
    BotOrientation.Theta += DeltaTheta;                                                                     // Update the bots orientation
    if ((BotOrientation.Theta > 3.15) || (BotOrientation.Theta < -3.15))
    {
      BotOrientation.Theta -= PI;
    };
  };
  void UpdateDesiredPose(double Theta, double X, double Y)
  {
    InputPose[0][0] = Theta; // Set the Theta distance to move
    InputPose[1][0] = X;
    InputPose[2][0] = Y;
  };
  void UpdateMotorObjects(void) // Updates motor objects with new target positions and calls the controller object to move them.
  {
    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    if (debug == true)
    {
      Serial.println("NewWheelSteps:");
      PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    };
    Controller.move(motor_1, motor_2, motor_3, motor_4);
  }
  void TestMotors(void)
  {
    motor_1.setTargetRel(200000);
    motor_2.setTargetRel(200);
    motor_3.setTargetRel(200);
    motor_4.setTargetRel(200);
    Controller.move(motor_1);
    // delay(500);
    // Controller.move(motor_2);
    // delay(500);
    // Controller.move(motor_3);
    // delay(500);
    // Controller.move(motor_4);
    // delay(500);
  };

private:
  //  MATRICIES FOR MATRIX CALCULATIONS================================================================================================
  mtx_type TestMove[3][1] = {
      {PI / 2},
      {25},
      {25}};
  volatile mtx_type InputPose[3][1] = {
      {0},
      {0},
      {0}};
  mtx_type HomogoenousMatrix[3][2] = {
      {0, 0},  //  DeltaX  Xcurr
      {0, 0},  //  DeltaY Ycurr
      {0, 1}}; //  0            0            1
  mtx_type RotationalMatrix[3][3] = {
      {1, 0, 0},  //  cos(Theta)  -sin(Theta)  0
      {0, 1, 0},  //  sin(Theta)  cos(Theta)   0
      {0, 0, 1}}; //  0            0            1
  mtx_type TranslationalMatrix1[3][3] = {
      {1, 0, 0},  //  1            0            deltaX
      {0, 1, 0},  //  0            1            deltaY
      {0, 0, 1}}; //  0            0            1
  mtx_type TranslationalMatrix2[3][3] = {
      {1, 0, 0},                       //  0            0          Xcurr
      {0, 1, 0},                       //  0            0          Ycurr
      {0, 0, 1}};                      //  0            0           1
  mtx_type T1[3][3] = {                // TransformationalMatrix = Trans1*Rot*Trans2
      {0, 0, 0},                       //
      {0, 0, 0},                       //
      {0, 0, 0}};                      //
  mtx_type T2[3][3] = {                // TransformationalMatrix = Trans1*Rot*Trans2
      {0, 0, 0},                       //
      {0, 0, 0},                       //
      {0, 0, 0}};                      //
  mtx_type TransformedMatrix[3][2] = { // TransformedMatrix = TransformationalMatrix*HomoMatrix
      {0, 0},
      {0, 0},
      {0, 0}};
  mtx_type ComplexStep[3][1] = {
      {0},
      {0},
      {0}};
  mtx_type NewWheelSpeeds[4][1] = {
      {0},
      {0},
      {0},
      {0}};
  mtx_type NewWheelSteps[4][1] = {
      {0},
      {0},
      {0},
      {0}};
  mtx_type WheelSteps[4][1] = {
      {0},
      {0},
      {0},
      {0}};
  const mtx_type InverseJacobian[4][3] = {
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT}};

  // OBJECTS FOR CONTROLLING STEPPER MOTORS================================================================================================
  Stepper motor_1;
  Stepper motor_2;
  Stepper motor_3;
  Stepper motor_4;
  Stepper *Motors[4] = {&motor_1, &motor_2, &motor_3, &motor_4};
  StepControl Controller; // Objects for rotating the motors to an endpoint in sync
};
