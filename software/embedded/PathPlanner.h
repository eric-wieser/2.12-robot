#pragma once

#include "Angle.h"
#include "Vector.h"
#include "Spline.h"

#include "RobotPosition.h"
class SerialCommunication;

/*
Class Name: PathPlanner
Description: Timed serial communication for data reporting to Matlab
Uesful public Member Variables: None
Public Member Functions:
    1. void initialize();
    2. void navigate(const RobotPosition &robotPos);
    3. void computeDesiredV();
External Function Dependency: None
External Variable Dependency:
    1.  void navigate(const RobotPosition &robotPos); with RobotPosition type object constant input passed by reference
*/
class PathPlanner {
  public:
    enum class Task {
        IDLE,
        TURN,
        STRAIGHT,
        DONE
    };
    Task currentTask;

    float desiredMVL;
    float desiredMVR;
    PathPlanner();

    /**
    update desiredMVL and desiredMVR to drive to the goal location
    this happens in two steps:
    1. turn to face the goal
    2. drive to it

    This function does not do all its work in one iteration, and must be called repeatedly

    Does not respect reportData.commandPhi
    */
    void turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData);

    /**
    adjusts desiredMVL and desiredMVR to stay at the current velocity, but steer towards the goal
    This function does not do all its work in one iteration, and must be called repeatedly
    */
    bool OrientationController(const RobotPosition & robotPos, const SerialCommunication & reportData);

  private:
    float pathGoal;
    Angle turnBegin;
    Angle turnEnd;
};
