#include "RobotPosition.h"

#include <math.h>
#include "constants.h"

//RobotPosition Class function implementation
RobotPosition::RobotPosition() {
  Phi = 0;
  X = 0;
  Y = 0;
  pathDistance = 0;
}

void RobotPosition::update(float dThetaL, float dThetaR) {
  float dPhi = (r / (2.0 * b)) * (dThetaR - dThetaL);
  Angle AvgPhi = Phi + dPhi / 2;
  float dX = (r / 2.0) * cos(AvgPhi) * (dThetaR + dThetaL);
  float dY = (r / 2.0) * sin(AvgPhi) * (dThetaR + dThetaL);

  X += dX;
  Y += dY;
  Phi += dPhi;
  pathDistance += sqrt(dX * dX + dY * dY);
}
