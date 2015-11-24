#include "PathPlanner.h"

#include "Arduino.h"
#include "constants.h"
#include "RobotPosition.h"
#include "SerialCommunication.h"
#include "util.h"

//PathPlanner Class function implementation
PathPlanner::PathPlanner() {
  currentTask = 0;
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

  if (currentTask == 0) { // waiting to receive command x,y
    desiredMVR = 0;
    desiredMVL = 0;
    if (!reportData.finished){
      currentTask = 1;
      turnBegin = robotPos.Phi;
      Vector dist = reportData.commandPos - robotPos.pos;

      // skip small movements
      if(dist.magnitudeSq() < 0.025*0.025) currentTask = 3;
      turnEnd = dist.angle();
    }
  }

  if (currentTask == 1) { //turn towards next point
    static const Spline turnSpline(0, 1, 0.3, 0.1);

    // interpolate our motor speeds quadratically in angle
    float through_turn = unlerp(turnBegin, turnEnd, robotPos.Phi);
    float speed = sgn(turnEnd  - turnBegin) * turnSpline.eval_dp(through_turn);

    desiredMVR = speed;
    desiredMVL = -speed;

    // move on
    if(through_turn >= 1 || turnBegin == turnEnd) {
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 2;
      pathGoal = robotPos.pathDistance + (reportData.commandPos - robotPos.pos).magnitude();
    }
  }

  if (currentTask == 2) { //now go straight to next point
    float speed = 0.4;

    // slow down near the goal
    const float slow_within = 0.2;
    const float slow_to = 0.15;
    float to_go = pathGoal - robotPos.pathDistance;
    if (to_go < slow_within)
      speed = lerp(slow_to, speed, to_go / slow_within);

    if (to_go > 0) {
      //if we aren't there yet, keep going
      desiredMVR = speed;
      desiredMVL = speed;
      bool done = OrientationController(robotPos, reportData);
      if(done) currentTask = 3;
    } else {
      currentTask = 3;
    }
  }
  if (currentTask == 3) { //done
    desiredMVR = 0;
    desiredMVL = 0;
    currentTask = 0;
    lastRobotPos = robotPos;
    Serial.println("NEXT POINT");
    reportData.updateStatus(true);
  }
}