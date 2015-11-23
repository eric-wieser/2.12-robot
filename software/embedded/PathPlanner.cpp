#include "PathPlanner.h"

#include "Arduino.h"
#include "constants.h"
#include "RobotPosition.h"
#include "SerialCommunication.h"

//PathPlanner Class function implementation
PathPlanner::PathPlanner() {
  currentTask = 1;
  desiredMVL = 0;
  desiredMVR = 0;
  phiGoal = 0;
}

void PathPlanner::LabTestRun(const RobotPosition & robotPos) {
  if (currentTask == 1) { //we are moving forward
    if (robotPos.pathDistance < 1.0) { //if we aren't there yet, keep going
      computeDesiredV(.2, 0);

    } else { //if we are there, stop, reset distances, and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 2;
    }
  }
  if (currentTask == 2) { //we are turning around
    if (robotPos.pathDistance < 1.0 + (PI * .25)) { //if we aren't there yet, keep going
      computeDesiredV(.1, 4);
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 3;
    }
  }
  if (currentTask == 3) { //we are going back home
    if (robotPos.pathDistance < 2.0 + (PI * .25) ) { //if we aren't there yet, keep going
      computeDesiredV(.2, 0);
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 4;
      Serial.println("DONE MOVING");
    }
  }
}

void PathPlanner::computeDesiredV(float forwardVel, float K) {
  //command velocities based off of K and average forwardVel
  desiredMVR = forwardVel * (1 + K * b);
  desiredMVL = forwardVel * (1 - K * b);
}

bool PathPlanner::OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData) {
  const float eps = .01;
  float KPhi = .50;

  Vector delta = reportData.commandPos - robotPos.pos;
  Angle phiGoal = delta.angle();
  float phiError = phiGoal - robotPos.Phi;

  if (abs(phiError) > PI/3){
    KPhi = 10.0;
  }

  desiredMVR += KPhi * phiError*2.0*b;
  desiredMVL -= KPhi * phiError*2.0*b;

  return delta.magnitudeSq() < eps*eps;
}

void PathPlanner::turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData) {
  //take next point (X,Y) positions given by MatLab, calculate phi to turn towards, then go straight
  Angle phiGoal = (reportData.commandPos - lastRobotPos.pos).angle();
  // constrain the error in phi to the [-pi, pi)
  float lastPhiError = phiGoal - Angle(lastRobotPos.Phi);
  float currentPhiError = phiGoal - Angle(robotPos.Phi);
  Vector dpos = reportData.commandPos - robotPos.pos;

  if (currentTask == 0) { // waiting to receive command x,y
    desiredMVR = 0;
    desiredMVL = 0;
    if (!reportData.finished){
      currentTask = 1;
    }
  }

  if (currentTask == 1) { //turn towards next point
    //turn counter clock wise
    if (lastPhiError > 0 && currentPhiError > 0) {
      desiredMVR = 0.2;
      desiredMVL = -0.2;
    }
    // turn clockwise
    else if (lastPhiError < 0 && currentPhiError < 0) {
      desiredMVR = -0.2;
      desiredMVL = 0.2;
    }
    // move on
    else {
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 2;
      pathGoal = robotPos.pathDistance + dpos.magnitude();
    }
  }

  if (currentTask == 2) { //now go straight to next point
    if (robotPos.pathDistance < pathGoal) { //if we aren't there yet, keep going
      computeDesiredV(.2, 0);
      bool done = OrientationController(robotPos, reportData);
      if(done) reportData.updateStatus(true);
    } else { //if we are there, stop and move on to the next task
      computeDesiredV(0, 0);
      currentTask = 0;
      lastRobotPos = robotPos;
      Serial.println("NEXT POINT");
      reportData.updateStatus(true);
    }
  }
}