#include <Arduino.h>
#include <TeensyStep.h>
#include <TeensyThreads.h>
#include <math.h>
#include "Adafruit_VL53L0X.h"
#include <functions.h>
#include <macros.h>
#include <InvJacobianClass.h>
#include <globals.h>

InvJacobian Jacobian;
void setup()
{
  InitSteppers();
}

void loop()
{
  double Origin[3][1] = {{0}, {0}, {0}};
  double One[3][1] = {{0}, {0}, {25}};
  double Two[3][1] = {{0}, {25}, {25}};
  double Three[3][1] = {{0}, {-25}, {-25}};
  double Four[3][1] = {{0}, {0}, {-25}};

  double SpinRt[3][1] = {{TWO_PI}, {0}, {0}};
  double SpinLft[3][1] = {{-TWO_PI}, {0}, {0}};

  double HalfSpinRt[3][1] = {{PI}, {0}, {0}};
  double HalfSpinLft[3][1] = {{-PI}, {0}, {0}};

  Jacobian.ComputeMoveAbs(Origin);
  controller.move(motor_1, motor_2, motor_3, motor_4);
  Jacobian.ComputeMoveAbs(HalfSpinRt);
  controller.move(motor_1, motor_2, motor_3, motor_4);
  Jacobian.ComputeMoveAbs(One);
  controller.move(motor_1, motor_2, motor_3, motor_4);
  Jacobian.ComputeMoveAbs(HalfSpinLft);
  controller.move(motor_1, motor_2, motor_3, motor_4);
}