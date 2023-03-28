#include <Arduino.h>
// #include <InvJacobian.h>
#include <StepperMotors.h>
#include <Map.h>
#include <math.h>
#include <TeensyStep.h>
#include <macros.h>
#include <Bounce2.h>
#include "Adafruit_VL53L1X.h"
#include "Adafruit_VL53L0X.h"

void MatrixMultiply(mtx_type *A, mtx_type *B, int m, int p, int n, mtx_type *C);
void ComputeMoveAbs(mtx_type ThetaXY[3][1]);

mtx_type InverseJacobian[4][3] = {
    {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, STEPS_PER_DIST_MULT},
    {-STEPS_PER_BOT_RAD, -STEPS_PER_DIST_MULT, -STEPS_PER_DIST_MULT}};

mtx_type BotPose[3][1] = {
    {0},
    {0},
    {0}};