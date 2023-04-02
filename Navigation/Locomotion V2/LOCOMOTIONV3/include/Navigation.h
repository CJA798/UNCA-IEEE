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
bool MoveTypeFlag = 0;
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
  bool MoveTypeArray[10];
  int WriteCounter = 0;
  int ReadCounter = 0;
  mtx_type MoveToRetrieve[3][1];

  void AddMove(bool MoveType, mtx_type Theta, mtx_type X, mtx_type Y)
  {
    WriteCounter++;
    MoveTypeArray[WriteCounter] = MoveType;
    MoveQueueArray[0][WriteCounter] = Theta;
    MoveQueueArray[1][WriteCounter] = X;
    MoveQueueArray[2][WriteCounter] = Y;

    Serial.print("WriteCounter: ");
    Serial.println(WriteCounter);
    if (WriteCounter >= 9)
    {
      WriteCounter = 0;
    };
  };
  // This function will first fill the MoveToRetrieve array with the data from the MoveQueueArray. It will then increment the ReadCounter. If the ReadCounter exceeds the bounds of the array, it will be reset to zero.
  // The function will return a pointer to the MoveToRetrieve array.
  bool GetMove(void) // function updates the global InputPose array with the next move in the queue. Returns 1 if there is a move in the queue, returns 0 if there is not a move in the queue.
  {                  // Updates InputPose to the next move in the queue.
    if (ReadCounter == WriteCounter)
    {
      return 0;
    };
    ReadCounter++;
    MoveTypeFlag = MoveTypeArray[ReadCounter];
    InputPose[0][0] = MoveQueueArray[0][ReadCounter];
    InputPose[1][0] = MoveQueueArray[1][ReadCounter];
    InputPose[2][0] = MoveQueueArray[2][ReadCounter];

    Serial.print("ReadCounter: ");
    Serial.println(ReadCounter);
    if (ReadCounter >= 9)
    {
      ReadCounter = 0;
    };
    return 1;
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

  void BlockingNavigationProcess(void) // This is the blocking version of our navigation process (AKA the function will not return untill the move is complete) I
  {                                    // The function does not need to check if we are moving; as it cannot exit until the move is complete.
    if (NewMoveQueue.GetMove())        // The function will also not run if there is no move in the queue.
      Serial.println("BOTPOSE");
    PrintMatrix((mtx_type *)BotPose, 3, 1);
    if (MoveTypeFlag == ROTATIONAL)
    {
      Serial.println("Rotational Move");
      ComputeMoveRotational(InputPose[0][0]);

      UpdateMotorObjects(BLOCKING); // Update the stepper objects with the new move
    };
    if (MoveTypeFlag == LINEAR)
    {
      Serial.println("Linear Move");
      ComputeMoveLinear(InputPose[1][0], InputPose[2][0]);

      UpdateMotorObjects(BLOCKING); // Update the stepper objects with the new move
    };
  };
  bool IsRunning(void)
  {
    return Controller.isRunning();
  };

  void ComputeMoveRotational(double Theta)
  {
    mtx_type DeltaXY[3][1] = {
        {0},
        {0},
        {0}};
    DeltaXY[0][0] = Theta - BotPose[0][0]; // Calculate the change in Theta
    BotPose[0][0] += Theta;                // Update the bot's pose with the new Theta
    Serial.println("DeltaXY: ");
    PrintMatrix((mtx_type *)DeltaXY, 3, 1);
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)DeltaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    Serial.println("WHEELSTEPS");
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    UpdateMotorObjects(1);

    if (BotPose[0][0] > (2 * PI)) // Check to see if our angle is greater than 2PI. If it is, subtract 2PI from it.
    {
      BotPose[0][0] -= (2 * PI);
    };
  }
  void ComputeMoveLinear(double X, double Y)
  {
    mtx_type DeltaXY[3][1] = {
        {0},
        {0},
        {0}};

    if (BotPose[0][0] != 0)                                                     // Checks to see if the bot is currently skewed at an angle.
    {                                                                           // if bot is skewed at an angle; calculate the actual desired movement vector. This is done by calculating the length of the vector, its angle, and then subtracting this angle from
                                                                                // the angle the bot is positioned at.
      Pose.fromCartesian(BotPose[1][0], BotPose[2][0]);                         // Update pose object with bots current pose
      NewMove.fromCartesian(X, Y);                                              // Update new move object with our new move
      TempPose.fromPolar(NewMove.getR(), NewMove.getAngle() - Pose.getAngle()); // Calculate the actual move given our bots current angle TempPose(theta) = NewMove(theta) - Pose(theta)
      X = TempPose.getX();                                                      // Update InputX = TempPose(X)
      Y = TempPose.getY();                                                      // Update InputY = TempPose(Y)
                                                                                // Update the bot's pose with the new Y
      Serial.println("NewAngle: ");
      Serial.println(NewMove.getAngle() - Pose.getAngle());
      Serial.println("New X: ");
      Serial.println(TempPose.getX());
      Serial.println("New Y: ");
      Serial.println(TempPose.getY());
      DeltaXY[1][0] = X - BotPose[1][0]; // Calculate the change in X
      DeltaXY[2][0] = Y - BotPose[2][0]; // Calculate the change in Y
      Serial.println("DeltaXY: ");
      PrintMatrix((mtx_type *)DeltaXY, 3, 1);
    }
    else
    {

      DeltaXY[1][0] = X - BotPose[1][0]; // Calculate the change in X
      DeltaXY[2][0] = Y - BotPose[2][0]; // Calculate the change in Y
      Serial.println("DeltaXY: ");
      PrintMatrix((mtx_type *)DeltaXY, 3, 1);
    };
    BotPose[1][0] = X; // Update the bot's pose with the new X
    BotPose[2][0] = Y;
    // MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps);
    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)DeltaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel
    Serial.println("WHEELSTEPS");
    PrintMatrix((mtx_type *)NewWheelSteps, 4, 1);
    UpdateMotorObjects(1);
  }
  void UpdateMotorObjects(char BlockingOrNot)
  {
    motor_1.setTargetAbs(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetAbs(NewWheelSteps[1][0]);
    motor_3.setTargetAbs(NewWheelSteps[2][0]);
    motor_4.setTargetAbs(NewWheelSteps[3][0]);

    switch (BlockingOrNot)
    {
    case BLOCKING:
      Controller.move(motor_1, motor_2, motor_3, motor_4);
      break;
    default:
      Controller.moveAsync(motor_1, motor_2, motor_3, motor_4);
      break;
    };
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
