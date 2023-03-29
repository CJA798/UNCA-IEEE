#include <main.h>

Stepper motor_1(MTR1DIR, MTR1STEP); // Stepper objects
Stepper motor_2(MTR2DIR, MTR2STEP);
Stepper motor_3(MTR3DIR, MTR3STEP);
Stepper motor_4(MTR4DIR, MTR4STEP);
StepControl controller; // Stepper controller object

// DEBOUNCE OBJECTS FOR SWITCHES
Bounce FrontLeftSw = Bounce();
Bounce FrontRightSw = Bounce();
Bounce BackRightSw = Bounce();
Bounce BackLeftSw = Bounce();
Bounce RightSideRightSw = Bounce();
Bounce RightSideLeftSw = Bounce();
Bounce LeftSideRightSw = Bounce();
Bounce LeftSideLeftSw = Bounce();

bool SwitchesState[8] = {0};
int i;
void InitSwitches(void)
{
  // THIS FUNCTION SETS THE PIN MODE FOR INPUT SWITCHES AND ATTACHES THEM TO THEIR DEBOUNCE OBJECT

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
  // delay(1000);
  // for (i = 0; i <= 7; i++)
  // {
  //   Serial.println(SwitchesState[i]);
  //   Serial.print(i);
  // }
}
void InitSteppers(void)
{

  // setup the motors
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
}

void setup()
{
  InitSwitches();
  Serial.begin(9600);
  InitSteppers();

  delay(5000);
}

void loop()
{
  // SwitchesProcess();

  double Origin[3][1] = {{0}, {0}, {50}};
  double One[3][1] = {{0}, {0}, {-50}};
  double Two[3][1] = {{0}, {25}, {0}};
  double Three[3][1] = {{0}, {25}, {25}};
  double Four[3][1] = {{0}, {-50}, {-50}};

  double SpinRt[3][1] = {{TWO_PI}, {0}, {0}};
  double SpinLft[3][1] = {{-TWO_PI}, {0}, {0}};

  double HalfSpinRt[3][1] = {{PI}, {0}, {0}};
  double HalfSpinLft[3][1] = {{-PI}, {0}, {0}};

  ComputeMoveAbs(Origin);
  controller.move(motor_1, motor_2, motor_3, motor_4);
  delay(3000);
  ComputeMoveAbs(One);
  controller.move(motor_1, motor_2, motor_3, motor_4);
  delay(3000);
  // ComputeMoveAbs(Two);
  // controller.move(motor_1, motor_2, motor_3, motor_4);
  // ComputeMoveAbs(Three);
  // controller.move(motor_1, motor_2, motor_3, motor_4);
  // ComputeMoveAbs(Four);
  // controller.move(motor_1, motor_2, motor_3, motor_4);
  //  ComputeMoveAbs(HalfSpinRt);
  // controller.move(motor_1, motor_2, motor_3, motor_4);
}

void ComputeMoveAbs(mtx_type ThetaXY[3][1]) // Will not let you make a complex move (i.e. spinning while moving)Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
// and updates the stepper objects with this distance. The controller object must be updated for the steppers to start this move. ROTATIONAL MOVES ARE ALWAYS RELATIVE.
//  Input is in form [ Theta ]
//                   [    X  ]
//                   [    Y  ]
{
  // if ((ThetaXY[0][0] >= 0) && ((ThetaXY[1][0] >= 0) || (ThetaXY[2][0] >= 0)))
  //{
  // Return without doing anything if a complex input is detected.
  // return;
  // Serial.println("ERROR: COMPLEX MOVE");
  //  }
  /*
    if (BotPose[0][0] > 0)
    { // if bot is skewed at an angle; calculate the actual desired movement vector. This is done by calculating the length of the vector, its angle, and then subtracting this angle from
      // the angle the bot is positioned at.
      double X = ThetaXY[1][0];
      double Y = ThetaXY[2][0];
      double R;
      double XYVectorAngle;
      if (X < 0)
      { // Quadrant 2
        R = sqrt((X * X) + (Y * Y));
        XYVectorAngle = atan(Y / X) + PI;
      }
      else if ((X < 0) && (Y < 0))
      { // Quadrant 3
        R = sqrt((X * X) + (Y * Y));
        XYVectorAngle = atan(Y / X) + PI;
      }
      else if (Y < 0)
      { // Quadrant 4
        R = sqrt((X * X) + (Y * Y));
        XYVectorAngle = atan(Y / X) + (2 * PI);
      }
      else
      { // Quadrant 1
        R = sqrt((X * X) + (Y * Y));
        XYVectorAngle = atan(Y / X);
      }
      X = R * cos(XYVectorAngle + BotPose[0][0]);
      Y = R * sin(XYVectorAngle + BotPose[0][0]);
      // ThetaXY[0][0] = 0;

      BotPose[0][0] += ThetaXY[0][0]; // Populate pose array
      BotPose[1][0] = ThetaXY[1][0];  // Populate pose array
      BotPose[2][0] = ThetaXY[2][0];  // Populate pose array

      ThetaXY[1][0] = X;
      ThetaXY[2][0] = Y;
    }
    else
    {

      BotPose[0][0] += ThetaXY[0][0]; // Populate pose array
      BotPose[1][0] = ThetaXY[1][0];  // Populate pose array
      BotPose[2][0] = ThetaXY[2][0];  // Populate pose array
    }
    Serial.println(ThetaXY[0][0]);
    Serial.println(ThetaXY[1][0]);
    Serial.println(ThetaXY[2][0]);
  */
  mtx_type NewWheelSteps[4][1];

  MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

  motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
  motor_2.setTargetRel(NewWheelSteps[1][0]);
  motor_3.setTargetRel(NewWheelSteps[2][0]);
  motor_4.setTargetRel(NewWheelSteps[3][0]);
  Serial.println(NewWheelSteps[0][0]);
}

// void TOFit(void)
//{
//   Wire.begin();
// TOF1.startRanging();
// vl53.setTimingBudget(15);
//}

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
