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

void PathPlanner::OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData) {
  float eps = .01;
  float KPhi = .50;
  float delY = reportData.commandY - robotPos.Y;
  float delX = reportData.commandX - robotPos.X;
  Angle phiGoal = atan2(delY, delX);
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
  Angle phiGoal = atan2((reportData.commandY - ylast), (reportData.commandX - xlast));
  // constrain the error in phi to the [-pi, pi)
  float lastPhiError = phiGoal - Angle(philast);
  float currentPhiError = phiGoal - Angle(robotPos.Phi);
  float dx = reportData.commandX - robotPos.X;
  float dy = reportData.commandY - robotPos.Y;

  if (currentTask == 0) { // waiting to receive command x,y
    desiredMVR = 0;
    desiredMVL = 0;
    if (!reportData.finished){
      currentTask = 1;
    }
  }
  if (currentTask == 1) { //turn towards next point
    if (lastPhiError > 0) { //turn counter clock wise
      if (currentPhiError >= 0){
        desiredMVR = 0.2;
        desiredMVL = -0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        pathGoal = robotPos.pathDistance + sqrt(dx*dx + dy*dy);
      }
    }
    if (lastPhiError < 0) { //turn clock wise
      if (currentPhiError <= 0){
        desiredMVR = -0.2;
        desiredMVL = 0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        pathGoal = robotPos.pathDistance + sqrt(dx*dx + dy*dy);
      }
    }
    if (lastPhiError == 0) { //don't need to turn
      currentTask = 2;
      pathGoal = robotPos.pathDistance + sqrt(dx*dx + dy*dy);
    }
  }

  if (currentTask == 2) { //now go straight to next point
    if (robotPos.pathDistance < pathGoal ) { //if we aren't there yet, keep going
      computeDesiredV(.2, 0);
      OrientationController(robotPos, reportData);
    } else { //if we are there, stop and move on to the next task
      computeDesiredV(0, 0);
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
    phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));

    if (robotPos.Phi > phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      computeDesiredV(.2, -4);
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight
      computeDesiredV(.2, 0);
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
      phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));
    if (robotPos.Phi < phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      computeDesiredV(.2, 4);
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight
      computeDesiredV(.2, 0);
    } else { //if we are there, stop and move on to the next task
      computeDesiredV(0, 0);
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
    }
  }
}
