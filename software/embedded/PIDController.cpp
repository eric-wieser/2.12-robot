#include "PIDController.h"

#include <math.h>      // for INFINITY
#include "constants.h" // for PERIOD
#include "Arduino.h"   // for constrain(v, lower, upper)

PIDController::PIDController(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd) {
  integratedError = 0;
  integralLimit = INFINITY;
}

float PIDController::update(float desV, float currV) {
  //Motor 1 velocity control
  integratedError += (desV - currV) * PERIOD;
  float integralCommand = constrain(ki * integratedError, -integralLimit, integralLimit);
  //derivativeVError1 = ((velocity - desiredMV1Prev) - (mV1 - mV1Prev))/dt;
  //derivativeCommand1 = Kdv1*derivativeVError1;
  return kp * (desV - currV) + kd * (0) + integralCommand;
}
