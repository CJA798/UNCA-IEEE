#include <TeensyStep.h>
// #include <TeensyThreads.h>
#include <math.h>
// #include "Adafruit_VL53L1X.h"

#define IRQ_PIN1 2
#define XSHUT_PIN1 1

#define X_STEP 29 // A0 from mega
#define X_DIR 30  // A1 "   "
#define Y_EN 12   // A2
#define Y_STEP 9  // A6
#define Y_DIR 10  // A7
#define Z_STEP 7  // D46
#define Z_DIR 8   // D48
#define E0_DIR 6  // D28
#define E0_STEP 5 // D26
// Adafruit_VL53L1X TOF1 = Adafruit_VL53L1X(XSHUT_PIN1, IRQ_P
Stepper motor_1(29, 30); // Stepper objects
Stepper motor_2(9, 10);
Stepper motor_3(7, 8);
Stepper motor_4(5, 6);
StepControl controller; // Stepper controller object

#define mtx_type double            // Matrix type definition
#define ONE_OVR_RT_2 (0.707106)    // (1/sqrt(2))
#define STEPS_PER_ROTATION (3200)  // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (25.46)       // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)         // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (1) // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (27)   // = (1/sqrt(2))*StepsPerCM*3 (gear ratio)

// Stepper Settings
#define MAX_MTR_SPEED (7500) // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (50)   // Steps/s^2 skips at around 15000

// GLOBALS

char KinematicsState;

mtx_type BotPose[3][1] = {
    {0},
    {0},
    {0}};
mtx_type InverseJacobian[4][3] = {
    {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT}};

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
    Serial.println("ERROR: COMPLEX MOVE");
    //  }

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

    mtx_type NewWheelSteps[4][1];

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    // Serial.println(NewWheelSpeed[0][0]);
}

void ComputeMoveRel(mtx_type ThetaXY[3][1]) // Computes bots movement distances for each stepper motor using the bots inverse jacobian matrix
// and updates the stepper objects with this distance. The controller object must be updated for the steppers to start this move.
//  Input is in form [ Theta ]
//                   [    X  ]
//                   [    Y  ]
{
    mtx_type NewWheelSteps[4][1];

    MatrixMultiply((mtx_type *)InverseJacobian, (mtx_type *)ThetaXY, 4, 3, 1, (mtx_type *)NewWheelSteps); // Multiply our position array with the jacobian matrix to get distances for each wheel

    motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
    motor_2.setTargetRel(NewWheelSteps[1][0]);
    motor_3.setTargetRel(NewWheelSteps[2][0]);
    motor_4.setTargetRel(NewWheelSteps[3][0]);
    // Serial.println(NewWheelSpeed[0][0]);
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

// THREADS AND INTERRUPTS//////////////////////////////////////////////////////////////////
// void kinematics_compute_thread(void)
//{
// switch (kinematicsState)
// {
// }
//}

void setup()
{
    pinMode(0, INPUT); // Input for start button
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    InitSteppers();
    // Serial.println("START");
    //  threads.addThread(kinematics_cqompute_thread);
}
int state = 0;
void loop()
{

    int buttonstate = digitalRead(0);
    Serial.println(buttonstate);
    if (buttonstate >= 1)
    {
        if (state == 1)
        {
            state = 0;
            digitalWrite(13, LOW);
        }
        else
        {
            state = 1;
            digitalWrite(13, HIGH);
        }
        delay(1000);
    }
    if (state == 1)
    {
        Serial.println("START");

        double Origin[3][1] = {{0}, {0}, {0}};
        double One[3][1] = {{0}, {0}, {25}};
        double Two[3][1] = {{0}, {25}, {0}};
        double Three[3][1] = {{0}, {25}, {25}};
        double Four[3][1] = {{0}, {-50}, {-50}};

        double SpinRt[3][1] = {{TWO_PI}, {0}, {0}};
        double SpinLft[3][1] = {{-TWO_PI}, {0}, {0}};

        double HalfSpinRt[3][1] = {{PI}, {0}, {0}};
        double HalfSpinLft[3][1] = {{-PI}, {0}, {0}};

        ComputeMoveAbs(Origin);
        // controller.move(motor_1, motor_2, motor_3, motor_4);
        // ComputeMoveAbs(One);
        // controller.move(motor_1, motor_2, motor_3, motor_4);
        // ComputeMoveAbs(Two);
        // controller.move(motor_1, motor_2, motor_3, motor_4);
        ComputeMoveAbs(Three);
        controller.move(motor_1, motor_2, motor_3, motor_4);
        ComputeMoveAbs(Four);
        controller.move(motor_1, motor_2, motor_3, motor_4);
        ComputeMoveAbs(HalfSpinRt);
        controller.move(motor_1, motor_2, motor_3, motor_4);
    }
}
