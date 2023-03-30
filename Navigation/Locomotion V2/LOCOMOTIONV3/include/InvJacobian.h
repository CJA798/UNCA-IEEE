#include <macros.h>
#include <TeensyStep.h>
#include <Bounce2.h>
#include <math.h>
#include <Coordinates.h>

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
  Bounce FrontLeftSw;
  Bounce FrontRightSw;
  Bounce BackRightSw;
  Bounce BackLeftSw;
  Bounce RightSideRightSw;
  Bounce RightSideLeftSw;
  Bounce LeftSideRightSw;
  Bounce LeftSideLeftSw;
  mtx_type InverseJacobian[4][3] = {
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
      {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT}};

public:
  mtx_type BotPose[3][1] = {
      {0},
      {0},
      {0}};

  bool SwitchesState[8] = {0};
  DriverObject() : motor_1(MTR1DIR, MTR1STEP),
                   motor_2(MTR2DIR, MTR2STEP),
                   motor_3(MTR3DIR, MTR3STEP),
                   motor_4(MTR4DIR, MTR4STEP),
                   Controller()
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
    Pose = Coordinates();
    NewMove = Coordinates();
    TempPose = Coordinates();
    FrontLeftSw = Bounce();
    FrontRightSw = Bounce();
    BackRightSw = Bounce();
    BackLeftSw = Bounce();
    RightSideRightSw = Bounce();
    RightSideLeftSw = Bounce();
    LeftSideRightSw = Bounce();
    LeftSideLeftSw = Bounce();
    pinMode(FRONT_LEFT_SWITCH, INPUT_PULLUP);
    pinMode(FRONT_RIGHT_SWITCH, INPUT_PULLUP);
    pinMode(RIGHTSIDE_LEFT_SWITCH, INPUT_PULLUP);
    pinMode(RIGHTSIDE_RIGHT_SWITCH, INPUT_PULLUP);
    pinMode(LEFTSIDE_LEFT_SWITCH, INPUT_PULLUP);
    pinMode(RIGHTSIDE_RIGHT_SWITCH, INPUT_PULLUP);
    pinMode(BACK_RIGHT_SWITCH, INPUT_PULLUP);
    pinMode(BACK_LEFT_SWTICH, INPUT_PULLUP);
    FrontRightSw.attach(FRONT_RIGHT_SWITCH); // 1
    FrontRightSw.interval(DEBOUNCE_TIME);
    FrontLeftSw.attach(FRONT_LEFT_SWITCH); // 2
    FrontLeftSw.interval(DEBOUNCE_TIME);
    BackRightSw.attach(BACK_RIGHT_SWITCH); // 3
    BackRightSw.interval(DEBOUNCE_TIME);
    BackLeftSw.attach(BACK_LEFT_SWTICH); // 3
    BackLeftSw.interval(DEBOUNCE_TIME);
    RightSideRightSw.attach(RIGHTSIDE_RIGHT_SWITCH); // 4
    RightSideRightSw.interval(DEBOUNCE_TIME);
    RightSideLeftSw.attach(RIGHTSIDE_LEFT_SWITCH); // 5
    RightSideLeftSw.interval(DEBOUNCE_TIME);
    LeftSideRightSw.attach(LEFTSIDE_RIGHT_SWITCH); // 6
    LeftSideRightSw.interval(DEBOUNCE_TIME);
    LeftSideLeftSw.attach(LEFTSIDE_LEFT_SWITCH); // 6
    LeftSideLeftSw.interval(DEBOUNCE_TIME);
  }

  void SwitchesProcess(void)
  {
    // Serial.println("SwitchesProcess");
    FrontRightSw.update();     // 1
    FrontLeftSw.update();      // 2
    BackLeftSw.update();       // 3
    BackRightSw.update();      // 4
    RightSideLeftSw.update();  // 5
    RightSideRightSw.update(); // 6
    LeftSideLeftSw.update();   // 7
    LeftSideRightSw.update();  // 8
    SwitchesState[0] = LeftSideRightSw.read();
    SwitchesState[1] = FrontLeftSw.read();
    SwitchesState[2] = FrontRightSw.read();
    SwitchesState[3] = RightSideLeftSw.read();
    SwitchesState[4] = RightSideRightSw.read();
    SwitchesState[5] = BackRightSw.read();
    SwitchesState[6] = BackLeftSw.read();
    SwitchesState[7] = LeftSideLeftSw.read();
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
    Serial.println(ThetaXY[0][0]);
    Serial.println(ThetaXY[1][0]);
    Serial.println(ThetaXY[2][0]);

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    Serial.println("WHEELSTEPS");
    Serial.println(NewWheelSteps[0][0]);
    Serial.println(NewWheelSteps[1][0]);
    Serial.println(NewWheelSteps[2][0]);
    Serial.println(NewWheelSteps[3][0]);
    Controller.move(motor_1, motor_2, motor_3, motor_4);
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
};
