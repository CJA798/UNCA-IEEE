#include <InvJacobian.h>

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
    /*
        motor_1.setTargetRel(NewWheelSteps[0][0]); // update the stepper object
        motor_2.setTargetRel(NewWheelSteps[1][0]);
        motor_3.setTargetRel(NewWheelSteps[2][0]);
        motor_4.setTargetRel(NewWheelSteps[3][0]);
        // Serial.println(NewWheelSpeed[0][0]);
    */
}

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

/*
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
*/

