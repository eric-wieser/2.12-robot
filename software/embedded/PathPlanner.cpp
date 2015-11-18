#include "PathPlanner.h"

#include "Arduino.h"
#include "constants.h"
#include "RobotPosition.h"
#include "SerialCommunication.h"

//PathPlanner Class function implementation
void PathPlanner::initialize() {
  currentTask = 1;
  K = 0;
  forwardVel = 0;
  desiredMVL = 0;
  desiredMVR = 0;
  phiGoal = 0;
  xlast = 0;
  ylast = 0;
  philast = 0;
  pathlast = 0;
  phiDesired = 0;
  pathDesired = 0;
}

void PathPlanner::LabTestRun(const RobotPosition & robotPos) {
  if (currentTask == 1) { //we are moving forward
    if (robotPos.pathDistance < 1.0) { //if we aren't there yet, keep going
      forwardVel = .2;
      K = 0;
      computeDesiredV();

    } else { //if we are there, stop, reset distances, and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 2;
    }
  }
  if (currentTask == 2) { //we are turning around
    if (robotPos.pathDistance < 1.0 + (PI * .25)) { //if we aren't there yet, keep going
      forwardVel = .1;
      K = 4;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 3;
    }
  }
  if (currentTask == 3) { //we are going back home
    if (robotPos.pathDistance < 2.0 + (PI * .25) ) { //if we aren't there yet, keep going
      forwardVel = .2;
      K = 0;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 4;
      Serial.println("DONE MOVING");
    }
  }
}

void PathPlanner::computeDesiredV() {
  //command velocities based off of K and average forwardVel
  desiredMVR = forwardVel * (1 + K * b);
  desiredMVL = forwardVel * (1 - K * b);
}

void PathPlanner::OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData) {
  float eps = .01;
  float KPhi = .50;
  float delY = reportData.commandY - robotPos.Y;
  float delX = reportData.commandX - robotPos.X;

  phiGoal = atan2((delY), (delX));
  float currentPhiError = fmod(phiGoal - robotPos.Phi + PI, 2*PI) - PI;
  if (currentPhiError > PI/3 || currentPhiError < PI/18){
    KPhi = 10.0;
  }

  desiredMVR += KPhi * currentPhiError*2.0*b;
  desiredMVL -= KPhi * currentPhiError*2.0*b;

  if( abs(delX) < eps && abs(delY) < eps) {
  reportData.updateStatus(true);
  }
}

void PathPlanner::turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData) {
  //take next point (X,Y) positions given by MatLab, calculate phi to turn towards, then go straight
  phiGoal = atan2((reportData.commandY - ylast), (reportData.commandX - xlast));
  //pathGoal = sqrt((reportData.commandX - xlast) * (reportData.commandX - xlast) + (reportData.commandY - ylast) * (reportData.commandY - ylast)) + pathlast;
  // constrain the error in phi to the [-pi, pi)
  float lastPhiError = fmod(phiGoal - philast + PI, 2*PI) - PI;
  float currentPhiError = fmod(phiGoal - robotPos.Phi + PI, 2*PI) - PI;
  if (currentTask == 0) { // waiting to receive command x,y
    desiredMVR = 0;
    desiredMVL = 0;
    if (!reportData.finished){
      currentTask = 1;
    }
  }
  if (currentTask == 1) { //turn towards next point
    if (lastPhiError > 0 || (lastPhiError <0 && 2*PI-abs(lastPhiError)<PI)){ //turn counter clock wise
      if (currentPhiError >= 0){
        desiredMVR = 0.2;
        desiredMVL = -0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        float pathAfterTurn = robotPos.pathDistance;
        pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));

      }
    }
    if (lastPhiError < 0 || (lastPhiError > 0 && 2*PI-abs(lastPhiError)<PI)){ //turn clock wise
      if (currentPhiError <= 0){
        desiredMVR = -0.2;
        desiredMVL = 0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        float pathAfterTurn = robotPos.pathDistance;
        pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));

      }
    }
    if (lastPhiError == 0) { //don't need to turn
      currentTask = 2;
      float pathAfterTurn = robotPos.pathDistance;
      pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));
    }
  }

  if (currentTask == 2) { //now go straight to next point
    if (robotPos.pathDistance < pathGoal ) { //if we aren't there yet, keep going
      forwardVel = .2;
      computeDesiredV();
      OrientationController(robotPos, reportData);
    } else { //if we are there, stop and move on to the next task
      forwardVel = 0;
      computeDesiredV();
      currentTask = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
      reportData.updateStatus(true);
    }
  }
}

void PathPlanner::turnAndGo(const RobotPosition & robotPos, const SerialCommunication & reportData) {
  //take next point (X,Y) positions given by MatLab, calculate phi to turn towards, then go forward and turn at the same time
  phiGoal = tan((reportData.commandY - ylast) / (reportData.commandX - xlast));

  if (philast > phiGoal){ //turn clock wise
    forwardVel = .2;
    phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));

    if (robotPos.Phi > phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      K = -4;
      computeDesiredV();
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight
      K=0;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
    }
  } else { //turn counter clock wise
      forwardVel = .2;
      phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));
    if (robotPos.Phi < phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      K = 4;
      computeDesiredV();
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight
      K=0;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
    }
  }
}
