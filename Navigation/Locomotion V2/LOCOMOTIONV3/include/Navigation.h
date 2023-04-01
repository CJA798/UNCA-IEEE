#include <macros.h>
#include <TeensyStep.h>
#include <math.h>
#include <Coordinates.h>
#include <Map.h>
#define mtx_type double // Matrix type definition

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

  mtx_type InverseJacobian[4][3] = {
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT}};

public:
  // PUBLIC VARIABLES FROM NAVIGATION PROCESS
  char NavState = 0;
  mtx_type InputPose[3][1] = {
      {0},
      {0},
      {0}};
  mtx_type BotPose[3][1] = {
      {0},
      {0},
      {0}};

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
    Pose = Coordinates();                // Initialize Pose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
    NewMove = Coordinates();             // Initialize NewMove Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
    TempPose = Coordinates();            // Initialize TempPose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
  }
  void NavigationProcess(void)
  {
    // This is the Process tbat is always called in the main loop, but does not run if the Navigation Process flag is not 1.
    if (NavState != 1)
    {
      return;
    };
    if (Controller.isRunning() != 0)
    {
      return; // Do not update motor controller object if the Bot is still moving
    }
    if (InputPose[0][0] > 0 && (InputPose[1][0] > 0 || InputPose[2][0] > 0)) // Blocking complex moves (i.e. spinning while moving) for now
    {
      Serial.println("ERROR: COMPLEX MOVE");
      return;
    }

    for (int i = 0; i < 3; i++)
    { // Update the bots pose array with the input pose array
      BotPose[i][0] += InputPose[i][0];
    };
    if (BotPose[0][0] > 2 * PI)
    {
      BotPose[0][0] -= 2 * PI;
    };
    if (InputPose[0][0] > 0)
      ComputeRotation(InputPose); // Use the compute rotation function if the input is a rotation
    else
      ComputeMoveAbs(InputPose); // Use the compute move function if the input is a linear move
  }
  void ComputeMoveAbs(mtx_type ThetaXY[3][1]) // Will not let you make a complex move (i.e. spinning while moving)Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
  // and updates the stepper objects with this distance. The controller object must be updated for the steppers to start this move. ROTATIONAL MOVES ARE ALWAYS RELATIVE.
  //  Input is in form [ Theta ]
  //                   [    X  ]
  //                   [    Y  ]
  {
    if (ThetaXY[0][0] != 0)
    {
      // Return without doing anything if theta is not zero
      Serial.println("ERROR: COMPLEX MOVE");
      return;
    }

    if (BotPose[0][0] > 0)
    { // if bot is skewed at an angle; calculate the actual desired movement vector. This is done by calculating the length of the vector, its angle, and then subtracting this angle from
      // the angle the bot is positioned at.
      Pose.fromCartesian(BotPose[1][0], BotPose[2][0]);                         // Update pose object with bots current pose
      NewMove.fromCartesian(ThetaXY[1][0], ThetaXY[2][0]);                      // Update new move object with our new move
      TempPose.fromPolar(NewMove.getR(), NewMove.getAngle() - Pose.getAngle()); // Calculate the actual move given our bots current angle

      ThetaXY[0][0] = 0;
      ThetaXY[1][0] = NewMove.getX();
      ThetaXY[2][0] = NewMove.getY();
    }
    // BotPose[0][0] += TempPose.getAngle();
    BotPose[1][0] = ThetaXY[1][0]; // Populate pose array
    BotPose[2][0] = ThetaXY[2][0]; // Populate pose array
    Serial.println("THETAXY");
    PrintMatrix((mtx_type *)ThetaXY, 3, 1);

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    Serial.println("WHEELSTEPS");
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    Controller.moveAsync(motor_1, motor_2, motor_3, motor_4);
  }
  void ComputeRotation(mtx_type ThetaXY[3][1])
  {
    BotPose[0][0] += ThetaXY[0][0];
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    motor_1.setTargetRel(NewWheelSteps[0][0]);                                                            // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    Serial.println("WHEELSTEPS ROTATION");
    Serial.println(NewWheelSteps[0][0]);
    Serial.println(NewWheelSteps[1][0]);
    Serial.println(NewWheelSteps[2][0]);
    Serial.println(NewWheelSteps[3][0]);
    Controller.moveAsync(motor_1, motor_2, motor_3, motor_4);
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
