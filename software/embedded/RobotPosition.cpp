#include "RobotPosition.h"

#include <math.h>
#include "constants.h"

//RobotPosition Class function implementation
void RobotPosition::initialize() {
  dPhi = 0;
  Phi = 0;
  prevPhi = 0;
  AvgPhi = 0;
  dX = 0;
  dY = 0;
  X = 0;
  Y = 0;
  prevX = 0;
  prevY = 0;
  pathDistance = 0;
}

void RobotPosition::update(float dThetaL, float dThetaR) {
  dPhi = (r / (2.0 * b)) * (dThetaR - dThetaL);
  Phi = prevPhi + dPhi;
  AvgPhi = (Phi + prevPhi) / 2.0;
  prevPhi = Phi;
  dX = (r / 2.0) * cos(AvgPhi) * (dThetaR + dThetaL);
  X = prevX + dX;
  dY = (r / 2.0) * sin(AvgPhi) * (dThetaR + dThetaL);
  Y = prevY + dY;
  prevX = X;
  prevY = Y;
  pathDistance  += sqrt(dX * dX + dY * dY);
}
