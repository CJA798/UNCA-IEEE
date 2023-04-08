#include <macros.h>
#include <TeensyStep.h>
#include <math.h>
#include <Coordinates.h>
#include <Map.h>
#include <Ports.h>
#include <iostream>
#include <MiscFunctions.h>
#include <iostream>
#include <array>
using namespace std;
bool debug = false;
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
Orientation BotOrientation;

elapsedMillis MoveTimer;
bool ComplexMoveStarted = IDLE;

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
  }
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
  }
  /*
    void ComputeNextStep(void) // Computes the desired incremental step (vector) based on our bots current position and orientation. and the global InputPose.
    // If we are skewed at an angle, we need to rotate the input vector to match the bot's orientation. (done via translational and rotational matrices)

    {
      double DeltaTheta = (InputPose[0][0] - BotOrientation.Theta)*PROPORTIONAL_DIVIDER; // Compute the X and Y distances to move
      double DeltaX = (InputPose[1][0] - BotOrientation.X)*PROPORTIONAL_DIVIDER;
      double DeltaY = (InputPose[2][0] - BotOrientation.Y)*PROPORTIONAL_DIVIDER;
      Serial.println("DELTAS");
      Serial.println(DeltaTheta);
      Serial.println(DeltaX);
      Serial.println(DeltaY);
      // If bot is not at an angle
      ComplexStep[0][0] = DeltaTheta;                                      // Set DeltaTheta
      ComplexStep[1][0] = DeltaX; // Set the X and Y distances to move
      ComplexStep[2][0] = DeltaY;

      BotOrientation.Theta += DeltaTheta; // Update the bots position
      BotOrientation.X += DeltaX;
      BotOrientation.Y += DeltaY;

      Serial.println("ComplexSteps");
      PrintMatrix((mtx_type *)ComplexStep, 3, 1);
      Serial.println("BotOrientation");
      Serial.println(BotOrientation.Theta);
      Serial.println(BotOrientation.X);
      Serial.println(BotOrientation.Y);
      MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ComplexStep, 4, 3, 1, (mtx_type *)NewWheelSpeeds);
    };
     complex move attempt
    void ComplexMove(void)
    {

      switch (ComplexMoveState)
      {
      case 0: // No move in InputPose

        break;
      case 1: // Starting input move
              // Serial.print("MoveTimer : ");
              //  Serial.println(MoveTimer);


        ComputeNextStep();
        Serial.println("NewWheelSpeeds");
        PrintMatrix((mtx_type *)NewWheelSpeeds, 4, 1);

        motor_1
            .setMaxSpeed(NewWheelSpeeds[0][0])
            .setAcceleration(COMPLEX_MOVE_ACCEL);
        motor_2
            .setMaxSpeed(NewWheelSpeeds[1][0])
            .setAcceleration(COMPLEX_MOVE_ACCEL);
        motor_3
            .setMaxSpeed(NewWheelSpeeds[2][0])
            .setAcceleration(COMPLEX_MOVE_ACCEL);
        motor_4
            .setMaxSpeed(NewWheelSpeeds[3][0])
            .setAcceleration(COMPLEX_MOVE_ACCEL);
        SpinMotor1.rotateAsync(motor_1);
        SpinMotor2.rotateAsync(motor_2);
        SpinMotor3.rotateAsync(motor_3);
        SpinMotor4.rotateAsync(motor_4);

        MoveTimer = 0; // Reset the move timer
        // we now must wait for time period to be up.
        ComplexMoveState = 3;
        break;
      case 2:
        int32_t mtr1Speed = SpinMotor1.getCurrentSpeed();
        int32_t mtr2Speed = SpinMotor2.getCurrentSpeed();
        int32_t mtr3Speed = SpinMotor3.getCurrentSpeed();
        int32_t mtr4Speed = SpinMotor4.getCurrentSpeed();

        SpinMotor1.overrideSpeed(NewWheelSpeeds[0][0] / mtr1Speed);
        SpinMotor2.overrideSpeed(NewWheelSpeeds[1][0] / mtr2Speed);
        SpinMotor3.overrideSpeed(NewWheelSpeeds[2][0] / mtr3Speed);
        SpinMotor4.overrideSpeed(NewWheelSpeeds[3][0] / mtr4Speed);
        ComplexMoveState = 3;
        break;
      case 3:
        // Serial.print("MoveTimer : ");
        // Serial.println(MoveTimer);
        if (MoveTimer >= MOVE_TIME_PERIOD)
        { // Waiting for time period to be up
          ComplexMoveState = 2;
          Serial.println("STATE 2 TO STATE 1");
        };
       // if ( Delta >=   0.1 || (InputPose[2][0] >= (BotOrientation.Y- .001))) || (InputPose[0][0] >= (BotOrientation.Theta- .1)))
       // { // Checking if the move is complete, if it is, we go to the "Do Nothing" State
        //  ComplexMoveState = 0;
        //  Serial.println("STATE 2 TO STATE 0");
       // };
        break;
      default:
        break;
      };
    }
    */
  void ComputeTranslation(void) // Process that will move the bot from BotOrientation to InputPose[3][1]. MoveState must be TRANSLATING to run.
  //  Input is in form [ Theta ]
  //                   [    X  ]
  //                   [    Y  ]
  {
    if (MoveState != TRANSLATING)
    {
      return;
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
    if (BotOrientation.Theta != 0)
    {                                                 // If the bot is at an ange
      TranslationalMatrix1[2][0] = BotOrientation.X;  // Set X
      TranslationalMatrix1[2][1] = BotOrientation.Y;  // Set Y
      TranslationalMatrix2[2][0] = -BotOrientation.X; // Set X
      TranslationalMatrix2[2][1] = -BotOrientation.Y; // Set Y
      HomogoenousMatrix[0][0] = DeltaX;               // Set X
      HomogoenousMatrix[0][1] = DeltaY;               // Set Y
      HomogoenousMatrix[1][0] = BotOrientation.X;
      HomogoenousMatrix[1][1] = BotOrientation.Y;
      RotationalMatrix[0][0] = cos(-BotOrientation.Theta);  // Set cos(Theta)
      RotationalMatrix[0][1] = -sin(-BotOrientation.Theta); // Set -sin(Theta)
      RotationalMatrix[1][0] = sin(-BotOrientation.Theta);  // Set sin(Theta)
      RotationalMatrix[1][1] = cos(-BotOrientation.Theta);  // Set cos(Theta)
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
      InputPose[0][0] = 0;
      InputPose[1][0] = TransformedMatrix[0][0];
      InputPose[2][0] = TransformedMatrix[1][0];
    }
    else if (BotOrientation.Theta == 0)
    {                           // If bot is not at an angle
      InputPose[1][0] = DeltaX; // Set the X and Y distances to move
      InputPose[2][0] = DeltaY;
    };

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)InputPose, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    UpdateMotorObjects();                                                                                   // Update the stepper objects
    BotOrientation.X += InputPose[1][0];                                                                    // Update the bots position
    BotOrientation.Y += InputPose[2][0];
  };
  void ComputeRotation(void) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  {
    if (MoveState != ROTATING)
    {
      return;
    };
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
    if ((BotOrientation.Theta > PI) || (BotOrientation.Theta < -PI))
    {
      BotOrientation.Theta -= PI;
    }
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
    motor_1.setTargetRel(200);
    motor_2.setTargetRel(200);
    motor_3.setTargetRel(200);
    motor_4.setTargetRel(200);
    Controller.move(motor_1);
    delay(500);
    Controller.move(motor_2);
    delay(500);
    Controller.move(motor_3);
    delay(500);
    Controller.move(motor_4);
    delay(500);
  };

private:
  //  MATRICIES FOR MATRIX CALCULATIONS================================================================================================
  mtx_type TestMove[3][1] = {
      {PI / 2},
      {25},
      {25}};
  mtx_type InputPose[3][1] = {
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
  mtx_type InverseJacobian[4][3] = {
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
