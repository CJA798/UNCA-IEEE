#include <macros.h>
#include <TeensyStep.h>
#include <math.h>
#include <Coordinates.h>
#include <Map.h>
#include <iostream>
using namespace std;
#define mtx_type float // Matrix type definition
#define BLOCKING (1)
#define NON_BLOCKING (0)
#define ROTATIONAL (1)
#define LINEAR (0)
#define MOTOR_SATURATION_SPEED (4000)
#define COMPLEX_MOVE_ACCEL (10000)
#define INPUT_VECT_PROPORTIONAL_RADUIS (100)
#define MOVE_TIME_PERIOD (300) // ms
struct Orientation
{
  double Theta; // Difference in angle between the bots current frame of reference
  // and the global frame of reference
  double X; // Position of the bots global frame of reference in the X direction
  double Y; // Position of the bots global frame of reference in the Y direction
};
// PUBLIC VARIABLES FROM NAVIGATION PROCESS
char NavState = 0; // A nav state of 0 means the bot is not moving. A nav state of 1 means the bot is
// making a linear move, a Nav state of 2 means the bot is making a rotational move.
bool IsMoving = 0;
Orientation BotOrientation;

elapsedMillis MoveTimer;
bool ComplexMoveStarted = 0;

class DriverObject
{
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
  StepControl Controller;   // Objects for rotating the motors to an endpoint in sync
  RotateControl SpinMotors; // Objects for rotating

public:
  //  CONSTRUCTOR FOR THE NAVIGATION PROCESS. THIS IS CALLED WHEN THE NAVIGATION PROCESS IS INITIALIZED.
  DriverObject() : motor_1(MTR1DIR, MTR1STEP), // MUST PASS CONSTRUCTOR OBJECTS FROM THE STEPPER CLASS
                   motor_2(MTR2DIR, MTR2STEP),
                   motor_3(MTR3DIR, MTR3STEP),
                   motor_4(MTR4DIR, MTR4STEP),
                   Controller(),
                   SpinMotors()
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
  }
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

  void ComputeNextStep(void) // Computes the desired incremental step (vector) based on our bots current position and orientation. and the global InputPose.
  // If we are skewed at an angle, we need to rotate the input vector to match the bot's orientation. (done via translational and rotational matrices)

  {

    Coordinates InputVector;
    InputVector.fromCartesian(InputPose[1][0], InputPose[2][0]);
    int Radius = INPUT_VECT_PROPORTIONAL_RADUIS;
    double ThetaInput = InputVector.getAngle();
    Coordinates ProportionalInputVect;
    ProportionalInputVect.fromPolar(Radius, ThetaInput); // ENSURES step is an incremental step based on INPUT_VECT_PROPORTIONAL_RADUIS

    if (BotOrientation.Theta != 0)
    {                                                                            // If the bot is at an angle, we need to rotate the input vector to match the bot's orientation
      TranslationalMatrix1[2][0] = BotOrientation.X;                             // Set X
      TranslationalMatrix1[2][1] = BotOrientation.Y;                             // Set Y
      TranslationalMatrix2[2][0] = -BotOrientation.X;                            // Set X
      TranslationalMatrix2[2][1] = -BotOrientation.Y;                            // Set Y
      HomogoenousMatrix[0][0] = ProportionalInputVect.getX() - BotOrientation.X; // Set DeltaX
      HomogoenousMatrix[0][1] = ProportionalInputVect.getY() - BotOrientation.Y; // Set DeltaY
      HomogoenousMatrix[1][0] = BotOrientation.X;
      HomogoenousMatrix[1][1] = BotOrientation.Y;
      RotationalMatrix[0][0] = cos(BotOrientation.Theta);  // Set cos(Theta)
      RotationalMatrix[0][1] = -sin(BotOrientation.Theta); // Set -sin(Theta)
      RotationalMatrix[1][0] = sin(BotOrientation.Theta);  // Set sin(Theta)
      RotationalMatrix[1][1] = cos(BotOrientation.Theta);  // Set cos(Theta)
      /* SERIAL PRINTS
      Serial.println("TranslationalMatrix1");
      PrintMatrix((mtx_type *)TranslationalMatrix1, 3, 3);
      Serial.println("TranslationalMatrix2");
      PrintMatrix((mtx_type *)TranslationalMatrix2, 3, 3);
      Serial.println("HomogoenousMatrix");
      PrintMatrix((mtx_type *)HomogoenousMatrix, 3, 2);
      Serial.println("RotationalMatrix");
      PrintMatrix((mtx_type *)RotationalMatrix, 3, 3);
      */
      // compute the matrix multiplications: T1 = Trans1*Rot , T2 = T1*Trans2, TransformedMatrix = T2*HomoMatrix
      MatrixMultiply((mtx_type *)TranslationalMatrix1, (mtx_type *)RotationalMatrix, 3, 3, 3, (mtx_type *)T1);
      MatrixMultiply((mtx_type *)T1, (mtx_type *)TranslationalMatrix2, 3, 3, 3, (mtx_type *)T2);
      MatrixMultiply((mtx_type *)T2, (mtx_type *)HomogoenousMatrix, 3, 3, 2, (mtx_type *)TransformedMatrix);
      /* Serial.println("T1");

      PrintMatrix((mtx_type *)T1, 3, 3);
     Serial.println("T2");
      PrintMatrix((mtx_type *)T2, 3, 3);

      Serial.println("TransformedMatrix");
      PrintMatrix((mtx_type *)TransformedMatrix, 3, 2);
*/ ComplexStep[0][0] = ProportionalInputVect.getAngle() - BotOrientation.Theta; // Set DeltaTheta
      ComplexStep[1][0] = TransformedMatrix[0][0];                              // Set the X and Y distances to move
      ComplexStep[2][0] = TransformedMatrix[1][0];
    }
    else if (BotOrientation.Theta == 0)                       // If bot isnt at an angle, no matrix math is needed. We simply the input position from the bots position
    {                                                         // If bot is not at an angle
      ComplexStep[0][0] = ProportionalInputVect.getAngle();   // Set DeltaTheta
      ComplexStep[1][0] = InputPose[1][0] - BotOrientation.X; // Set the X and Y distances to move
      ComplexStep[2][0] = InputPose[1][0] - BotOrientation.Y;
    };
    BotOrientation.Theta += ProportionalInputVect.getAngle(); // Update the bots position
    BotOrientation.X += ProportionalInputVect.getX();
    BotOrientation.Y += ProportionalInputVect.getY();
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ComplexStep, 4, 3, 1, (mtx_type *)NewWheelSpeeds);
  };
  void ComplexMove(void)
  {
    if ((InputPose[1][0] == BotOrientation.X) & (InputPose[2][0] == BotOrientation.Y) & (InputPose[0][0] == BotOrientation.Theta))
    {
      return; // If the bot is has reached the desired position, return.
    };
    if(ComplexMoveStarted == 0){ // If this is the first time we are calling this function
      MoveTimer = 0; // Reset the move timer
      ComplexMoveStarted = 1; // Set the move started flag to 1
    };
    if(MoveTimer < MOVE_TIME_PERIOD){
      return; // If the move timer has not reached the time period, return.
    };
    MoveTimer += MOVE_TIME_PERIOD; // Increment the move timer
    ComputeNextStep();
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
    SpinMotors.rotateAsync(Motors);
    // Wait for time period to be up.
  }
  void ComputeTranslation(double X, double Y) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  // and updates the stepper objects with this distance. The controller object must be updated for the steppers to start this move.
  //  Input is in form [ Theta ]
  //                   [    X  ]
  //                   [    Y  ]
  {
    Serial.println("Compute Translation");
    Serial.println("Bot Pose");
    Serial.print("Theta: ");
    Serial.println(BotOrientation.Theta);
    Serial.print("X: ");
    Serial.println(BotOrientation.X);
    Serial.print("Y: ");
    Serial.println(BotOrientation.Y);

    if (BotOrientation.Theta != 0)
    {                                                 // If the bot is at an ange
      TranslationalMatrix1[2][0] = BotOrientation.X;  // Set X
      TranslationalMatrix1[2][1] = BotOrientation.Y;  // Set Y
      TranslationalMatrix2[2][0] = -BotOrientation.X; // Set X
      TranslationalMatrix2[2][1] = -BotOrientation.Y; // Set Y
      HomogoenousMatrix[0][0] = X;                    // Set X
      HomogoenousMatrix[0][1] = Y;                    // Set Y
      HomogoenousMatrix[1][0] = BotOrientation.X;
      HomogoenousMatrix[1][1] = BotOrientation.Y;
      RotationalMatrix[0][0] = cos(BotOrientation.Theta);  // Set cos(Theta)
      RotationalMatrix[0][1] = -sin(BotOrientation.Theta); // Set -sin(Theta)
      RotationalMatrix[1][0] = sin(BotOrientation.Theta);  // Set sin(Theta)
      RotationalMatrix[1][1] = cos(BotOrientation.Theta);  // Set cos(Theta)
      Serial.println("TranslationalMatrix1");
      PrintMatrix((mtx_type *)TranslationalMatrix1, 3, 3);
      Serial.println("TranslationalMatrix2");
      PrintMatrix((mtx_type *)TranslationalMatrix2, 3, 3);
      Serial.println("HomogoenousMatrix");
      PrintMatrix((mtx_type *)HomogoenousMatrix, 3, 2);
      Serial.println("RotationalMatrix");
      PrintMatrix((mtx_type *)RotationalMatrix, 3, 3);

      // compute the matrix multiplications: T1 = Trans1*Rot , T2 = T1*Trans2, TransformedMatrix = T2*HomoMatrix
      MatrixMultiply((mtx_type *)TranslationalMatrix1, (mtx_type *)RotationalMatrix, 3, 3, 3, (mtx_type *)T1);
      Serial.println("T1");
      PrintMatrix((mtx_type *)T1, 3, 3);
      MatrixMultiply((mtx_type *)T1, (mtx_type *)TranslationalMatrix2, 3, 3, 3, (mtx_type *)T2);
      Serial.println("T2");
      PrintMatrix((mtx_type *)T2, 3, 3);

      MatrixMultiply((mtx_type *)T2, (mtx_type *)HomogoenousMatrix, 3, 3, 2, (mtx_type *)TransformedMatrix);
      Serial.println("TransformedMatrix");
      PrintMatrix((mtx_type *)TransformedMatrix, 3, 2);

      InputPose[0][0] = 0;
      InputPose[1][0] = TransformedMatrix[0][0];
      InputPose[2][0] = TransformedMatrix[1][0];
      Serial.println("TransformedMatrix ");
    }
    else if (BotOrientation.Theta == 0)
    {                      // If bot is not at an angle
      InputPose[1][0] = X; // Set the X and Y distances to move
      InputPose[2][0] = Y;
    };
    Serial.println("Theta: ");
    // PrintMatrix((mtx_type *)InputPose, 3, 1);
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)InputPose, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    UpdateMotorObjects();                                                                                   // Update the stepper objects
    BotOrientation.X += InputPose[1][0];                                                                    // Update the bots position
    BotOrientation.Y += InputPose[2][0];
  };
  void ComputeRotation(double Theta) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  {
    Serial.println("Rotation");
    Serial.println("Bot Pose");
    Serial.print("Theta: ");
    Serial.println(BotOrientation.Theta);
    Serial.print("X: ");
    Serial.println(BotOrientation.X);
    Serial.print("Y: ");
    Serial.println(BotOrientation.Y);
    InputPose[0][0] = Theta; // Set the Theta distance to move
    InputPose[1][0] = 0;
    InputPose[2][0] = 0;
    Serial.println("Theta: ");
    // PrintMatrix((mtx_type *)InputPose, 3, 1);

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)InputPose, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    UpdateMotorObjects();                                                                                   // Update the stepper objects
    BotOrientation.Theta -= Theta;                                                                          // Update the bots orientation
    if ((BotOrientation.Theta > PI) || (BotOrientation.Theta < -PI))
    {
      BotOrientation.Theta -= PI;
    }
  };
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
  /*
  void ComputeTranslation(mtx_type ThetaXY[3][1])
  {
    if(ThetaXY[0][0] == 0){
      Serial.println("ERROR: COMPLEX MOVE");
    }
    ThetaXY[0][0] = 0;

    mtx_type NewWheelSteps[4][1];
    Serial.println(ThetaXY[0][0]);
    Serial.println(ThetaXY[1][0]);
    Serial.println(ThetaXY[2][0]);

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    // Serial.println(NewWheelSpeed[0][0]);

    Controller.move(motor_1, motor_2, motor_3, motor_4);
  }
  */
  void UpdateMotorObjects(void)
  {

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    Serial.println("NewWhelSteps:");
    // Serial.print(NewWheelSteps[0][0]);
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    Controller.move(motor_1, motor_2, motor_3, motor_4);
  }

  void MatrixMultiply(mtx_type *A, mtx_type *B, int m, int p, int n, mtx_type *C)
  {
    // A = input matrix (m x p)
    // B = input matrix (p x n)
    // m = number of rows in A
    // p = number of columns in A = number of rows in B
    // n = number of columns in B
    // C = output matrix = A*B (m x n)
    int i, j, k;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
      {
        C[n * i + j] = 0;
        for (k = 0; k < p; k++)
          C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
      }
  }

  void PrintMatrix(mtx_type *A, int m, int n)
  {
    // A = input matrix (m x n)
    // m = number of rows in A
    // n = number of columns in A
    int i, j;
    for (i = 0; i < m; i++)
    {
      for (j = 0; j < n; j++)
        Serial.print(A[n * i + j], 4);
      Serial.print(" ");
      Serial.println();
    }
  }
};
