#include "MotorController.h"

#include "constants.h"

#include "Drive.h"

const float maxSpeed = 0.44;

MotorController::MotorController(Drive& d) : drive(d) {
  mRCommand = 0;
  mLCommand = 0;
  mRController.integralLimit = mLController.integralLimit = 200;
}

void MotorController::controlMR(float desV, float currV) {
  desV = constrain(desV,-maxSpeed, maxSpeed);
  mRCommand += mRController.update(desV, currV);
  drive.setMRSpeed(constrain(mRCommand, -400, 400));
}
void MotorController::controlML(float desV, float currV) {
  desV = constrain(desV,-maxSpeed, maxSpeed);
  mLCommand += mLController.update(desV, currV);
  drive.setMLSpeed(constrain(mLCommand, -400, 400));
}