#include "PathPlanner.h"

#include "RobotPosition.h"
#include "SerialCommunication.h"
#include "constants.h"

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

bool PathPlanner::OrientationController(const RobotPosition & robotPos,
                                        const SerialCommunication & reportData,
                                        float kp) {
  float distEps = 0.025; //2.5 cm

  float dx = reportData.commandX - robotPos.X;
  float dy = reportData.commandY - robotPos.Y;
  bool closeEnough = dx*dx + dy*dy > distEps*distEps;
  if(closeEnough) {
    return true;
  }
  phiGoal = atan2(dy, dx);

  // P controller
  const float unsaturatedKphi = 0.41 / PI;  // full speed at full error
  float KPhi = unsaturatedKphi * kp;
  float currentPhiError = phiGoal - robotPos.Phi;

  // normalize error within [-PI, PI)
  while(currentPhiError >= PI) currentPhiError -= PI;
  while(currentPhiError < -PI) currentPhiError += PI;

  // modify desired velocities
  float deltaVel = currentPhiError * KPhi;
  desiredMVR += deltaVel;
  desiredMVL -= deltaVel;

  return false;
}

void PathPlanner::turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData) {
  if(currentTask == 1) {
    // the robot is waiting to receive command (x,y)
    desiredMVR = desiredMVL = 0;

    if(!reportData.finished) currentTask = 2;
  }
  else if(currentTask == 2) {
    desiredMVL = desiredMVR = 0;
    OrientationController(robotPos, reportData, 8);

    const float phiEps = PI / 180 * 2.5;

    if(abs(phiGoal - robotPos.Phi) < phiEps) currentTask = 3;
  }
  else if(currentTask == 3) {
    // the robot is going straight toward the next way point
    desiredMVL = 0.3;
    desiredMVR = 0.3;
    bool done = OrientationController(robotPos, reportData, 2);
    reportData.updateStatus(done);

    if(reportData.finished) currentTask = 1;
  }
  /* Here separate task into three cases:
  if currentTask = 1, 
  if currentTask = 2, the robot is turning to face the next way point
  if currentTask = 3, the robot is going straight toward the next way point

  Note for task 1, you need a condition to check whether a new way point is received

  For task 2, use lastPhiError to check if the robot need to turn clockwise or counter clockwise.
  If there is lastPhiError = 0, then the robot does not need to turn.

  End with a condition that tells us that the way point has been reached
  e.g., reportData.updateStatus(true);
  */
}