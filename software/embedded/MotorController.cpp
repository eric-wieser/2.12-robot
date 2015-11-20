#include "MotorController.h"

#include "constants.h"

//PIController Class function implementation
void MotorController::initialize() {
  md.init();
  m1Command = 0;
  m2Command = 0;
  m1Controller.integralLimit = m2Controller.integralLimit = 200;
}

void MotorController::controlM1(float desV, float currV) {
  desV = constrain(desV,-0.41, 0.41);
  m1Command += m1Controller.update(desV, currV);
  md.setM1Speed(constrain(m1Command, -400, 400));
}
void MotorController::controlM2(float desV, float currV) {
  desV = constrain(desV,-0.41, 0.41);
  m2Command += m2Controller.update(desV, currV);
  md.setM1Speed(constrain(m2Command, -400, 400));
}