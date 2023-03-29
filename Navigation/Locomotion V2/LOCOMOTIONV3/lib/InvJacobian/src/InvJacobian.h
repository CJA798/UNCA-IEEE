// #include <macros.h>
#include <Arduino.h>
#include <TeensyStep.h>
#define MTR1STEP 43
#define MTR1DIR 42
#define MTR2STEP 51
#define MTR2DIR 46
#define MTR3STEP 45
#define MTR3DIR 44
#define MTR4STEP 53
#define MTR4DIR 52
#define STORAGE_MTRSTEP 40
#define STORAGE_MTRDIR 41

#define mtx_type double            // Matrix type definition
#define ONE_OVR_RT_2 (0.707106)    // (1/sqrt(2))
#define STEPS_PER_ROTATION (3200)  // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (25.46)       // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)         // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (859.44) // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (27)   // = (1/sqrt(2))*StepsPerCM*3 (gear ratio)

// Stepper Settings
#define MAX_MTR_SPEED (7500) // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (50)   // Steps/s^2 skips at around 15000

void MatrixMultiply(mtx_type *A, mtx_type *B, int m, int p, int n, mtx_type *C);
void ComputeMoveAbs(mtx_type ThetaXY[3][1]);
void ComputeMoveRel(mtx_type ThetaXY[3][1]);
