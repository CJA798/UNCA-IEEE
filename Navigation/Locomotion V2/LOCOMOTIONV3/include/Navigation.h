#include <macros.h>
#include <TeensyStep.h>
#include <math.h>
#include <Coordinates.h>
#include <Map.h>
#include <iostream>
using namespace std;
#define mtx_type double // Matrix type definition
#define BLOCKING (1)
#define NON_BLOCKING (0)
#define ROTATIONAL (1)
#define LINEAR (0)
mtx_type InputPose[3][1] = {
    {0},
    {0},
    {0}};
mtx_type BotPose[3][1] = {
    {0},  // Theta
    {0},  // X BotPose[1][0] = 122;
    {0}}; // Y
// PUBLIC VARIABLES FROM NAVIGATION PROCESS
char NavState = 0; // A nav state of 0 means the bot is not moving. A nav state of 1 means the bot is
// making a linear move, a Nav state of 2 means the bot is making a rotational move.
bool IsMoving = 0;

class DriverObject
{
private:
  Coordinates Pose;
  Coordinates NewMove;
  Coordinates TempPose;
  Stepper motor_1;
  Stepper motor_2;
  Stepper motor_3;
  Stepper motor_4;
  StepControl Controller;
  mtx_type NewWheelSteps[4][1];
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

public:

  //  CONSTRUCTOR FOR THE NAVIGATION PROCESS. THIS IS CALLED WHEN THE NAVIGATION PROCESS IS INITIALIZED.
  DriverObject() : motor_1(MTR1DIR, MTR1STEP), // MUST PASS CONSTRUCTOR OBJECTS FROM THE STEPPER CLASS
                   motor_2(MTR2DIR, MTR2STEP),
                   motor_3(MTR3DIR, MTR3STEP),
                   motor_4(MTR4DIR, MTR4STEP),
                   Controller() // MUST PASS CONSTRUCTOR OBJECTS FROM THE CONTROLLER CLASS
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
   // Pose = Coordinates();                // Initialize Pose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
   // NewMove = Coordinates();             // Initialize NewMove Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
   // TempPose = Coordinates();            // Initialize TempPose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
  }
  void ComputeMove(mtx_type ThetaXY[3][1]) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  // and updates the stepper objects with this distance. The controller object must be updated for the steppers to start this move.
  //  Input is in form [ Theta ]
  //                   [    X  ]
  //                   [    Y  ]
  {
    Serial.println(ThetaXY[0][0]);
    Serial.println(ThetaXY[1][0]);
    Serial.println(ThetaXY[2][0]);
    Serial.println("new:");
    BotPose[0][0] = ThetaXY[0][0]; // Populate pose array
    BotPose[1][0] = ThetaXY[1][0];
    BotPose[2][0] = ThetaXY[2][0];

    mtx_type NewWheelSteps[4][1];

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    
      motor_1.setTargetAbs(NewWheelSteps[0][0]); // update the stepper object
      motor_2.setTargetAbs(NewWheelSteps[1][0]);
      motor_3.setTargetAbs(NewWheelSteps[2][0]);
      motor_4.setTargetAbs(NewWheelSteps[3][0]);
      Controller.move(motor_1, motor_2, motor_3, motor_4);
      /*
          motor_1.setPosition(PreviousSteps[0]); // Restore steps after rotational move
          motor_2.setPosition(PreviousSteps[1]);
          motor_3.setPosition(PreviousSteps[2]);
          motor_4.setPosition(PreviousSteps[3]);
          */
  }
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
  void UpdateMotorObjects(char BlockingOrNot)
  {
    motor_1.setTargetAbs(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetAbs(NewWheelSteps[1][0]);
    motor_3.setTargetAbs(NewWheelSteps[2][0]);
    motor_4.setTargetAbs(NewWheelSteps[3][0]);
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
      Serial.println();
    }
  }
};
