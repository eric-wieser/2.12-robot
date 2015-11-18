#include "PIController.h"

#include "constants.h"

//PIController Class function implementation
void PIController::initialize() {
  md.init();
  m1Command = 0;
  m2Command = 0;
  PrevCommand1 = 0;
  PrevCommand2 = 0;
  integratedVError1 = 0;
  integratedVError2 = 0;
  integralCommand1 = 0;
  integralCommand2 = 0;
}

void PIController::doPIControl(String side, float desV, float currV) {
  desV = constrain(desV,-0.41, 0.41);
  if (side == "Right") {
    //Motor 1 velocity control
    integratedVError1 = integratedVError1 + (desV - currV) * PERIOD;
    integralCommand1 = constrain(Kiv1 * integratedVError1, -200, 200);
    //integralCommand1 = Kiv1*integratedVError1;
    //derivativeVError1 = ((velocity - desiredMV1Prev) - (mV1 - mV1Prev))/dt;
    //derivativeCommand1 = Kdv1*derivativeVError1;
    m1Command = Kpv1 * (desV - currV) + Kdv1 * (0) + integralCommand1;
    m1Command += PrevCommand1;
    md.setM1Speed(constrain(m1Command, -400, 400));
    PrevCommand1 = m1Command;
  }
  else if (side == "Left") {
    //Motor 2 velocity control
    integratedVError2 = integratedVError2 + (desV - currV) * PERIOD;
    integralCommand2 = constrain(Kiv2 * integratedVError2, -200, 200);
    m2Command = Kpv2 * (desV - currV) + Kdv2 * (0) + integralCommand2;
    m2Command += PrevCommand2;
    md.setM2Speed(constrain(-1 * m2Command, -400, 400));
    PrevCommand2 = m2Command;
  }
  else {
    md.setM1Speed(0);
    md.setM2Speed(0);
    Serial.println("ERROR: INVALID MOTOR CHOICE GIVEN TO PI CONTROLLER");
  }
}
