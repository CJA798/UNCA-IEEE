#include <macros.h>
#include <TeensyStep.h>
#include <math.h>
#include <Coordinates.h>
#include <Map.h>
#include <iostream>
using namespace std;
#define mtx_type double // Matrix type definition

mtx_type InputPose[3][1] = {
    {0},
    {0},
    {0}};
mtx_type BotPose[3][1] = {
    {0},
    {0},
    {0}};
class MoveQueue
{ /*This is a class that creates an 3x10 array that contains a queue of new moves for our robotics platform. The data will be accessed using a Read counter and a write counter.
When the read counter is equal to the write counter, the Navigation Process flag is zero; when the read counter is not equal to the write counter, the Navigation Process flag is one.
The Navigation Process will read the data from the array and then increment the read counter. The Navigation Process will then check if the read counter is equal to the write counter.
if the read counter or the write counter exceeds the bounds of the array, is is reset to zero.
This class will contain the functions AddMove(Theta, X, Y) and GetMove(); Addmove will use the write counter to traverse the second dimension of the array, while the first
*/

private:
public:
  mtx_type MoveQueueArray[3][10];
  int WriteCounter = 0;
  int ReadCounter = 0;
  mtx_type MoveToRetrieve[3][1];

  void AddMove(mtx_type Theta, mtx_type X, mtx_type Y)
  {
    MoveQueueArray[0][WriteCounter] = Theta;
    MoveQueueArray[1][WriteCounter] = X;
    MoveQueueArray[2][WriteCounter] = Y;
    WriteCounter++;
    Serial.print("WriteCounter: ");
    Serial.println(WriteCounter);
    if (WriteCounter >= 10)
    {
      WriteCounter = 0;
    };
  };
  // This function will first fill the MoveToRetrieve array with the data from the MoveQueueArray. It will then increment the ReadCounter. If the ReadCounter exceeds the bounds of the array, it will be reset to zero.
  // The function will return a pointer to the MoveToRetrieve array.
  void GetMove()
  { // Updates InputPose to the next move in the queue.
    InputPose[0][0] = MoveQueueArray[0][ReadCounter];
    InputPose[1][0] = MoveQueueArray[1][ReadCounter];
    InputPose[2][0] = MoveQueueArray[2][ReadCounter];
    ReadCounter++;
    Serial.print("ReadCounter: ");
    Serial.println(ReadCounter);
    if (ReadCounter >= 10)
    {
      ReadCounter = 0;
    };
  };
};

class DriverObject
{
private:
  // MoveQueue* FirstMove;
  // MoveQueue* LastMove;
 
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
  MoveQueue NewMoveQueue;
  // PUBLIC VARIABLES FROM NAVIGATION PROCESS
  char NavState = 0; // A nav state of 0 means the bot is not moving. A nav state of 1 means the bot is
  // making a linear move, a Nav state of 2 means the bot is making a rotational move.
  bool IsMoving = 0;
 

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
    NewMoveQueue = MoveQueue();          // Initialize the MoveQueue object
    Pose = Coordinates();                // Initialize Pose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
    NewMove = Coordinates();             // Initialize NewMove Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
    TempPose = Coordinates();            // Initialize TempPose Coordinates object. This is for using the Cordinates library to do polar to cart conversions.
  }
  void NavigationProcess(void)
  {
    if (NewMoveQueue.ReadCounter != NewMoveQueue.WriteCounter)
    { // Check if the move queue read counters and write
      NewMoveQueue.GetMove(); // Get the next move from the move queue
      IsMoving = 1;
    }
    else{
      return;
    }
    ComputeMoveAbs(InputPose);  // Use the compute move function if the input is a move
    UpdateMotorObjects();       // Update the stepper objects with the new move

  };
  bool IsRunning(void)
  {
    return Controller.isRunning();
  };

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
    /*
    if (BotPose[0][0] > 0)
    { // if bot is skewed at an angle; calculate the actual desired movement vector. This is done by calculating the length of the vector, its angle, and then subtracting this angle from
      // the angle the bot is positioned at.
      Pose.fromCartesian(BotPose[1][0], BotPose[2][0]);                         // Update pose object with bots current pose
      NewMove.fromCartesian(ThetaXY[1][0], ThetaXY[2][0]);                      // Update new move object with our new move
      TempPose.fromPolar(NewMove.getR(), NewMove.getAngle() - Pose.getAngle()); // Calculate the actual move given our bots current angle
    };
    */
    Serial.println("THETAXY");
    PrintMatrix((mtx_type *)ThetaXY, 3, 1);
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    Serial.println("WHEELSTEPS");
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    UpdateMotorObjects();
  }
  void ComputeRotation(mtx_type ThetaXY[3][1])
  {
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    Serial.println("WHEELSTEPS ROTATION");
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    UpdateMotorObjects();
  }
  void UpdateMotorObjects(void)
  {
    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
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
