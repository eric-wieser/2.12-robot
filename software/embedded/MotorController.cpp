#include "MotorController.h"

#include "constants.h"

#include "Drive.h"

const float maxSpeed = 0.44;
const int maxPower = 400;

MotorController::MotorController(Drive& d) : drive(d) {
  mRCommand = 0;
  mLCommand = 0;
  mRController.integralLimit = mLController.integralLimit = 200;
}

void MotorController::controlMR(float desV, float currV) {
  desV = constrain(desV,-maxSpeed, maxSpeed);
  mRCommand += mRController.update(desV, currV);
  mRCommand = constrain(mRCommand, -maxPower, maxPower);
  drive.setMRSpeed(mRCommand);
}
void MotorController::controlML(float desV, float currV) {
  desV = constrain(desV,-maxSpeed, maxSpeed);
  mLCommand += mLController.update(desV, currV);
  mLCommand = constrain(mLCommand, -maxPower, maxPower);
  drive.setMLSpeed(mLCommand);
}