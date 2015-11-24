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
    int currentTask;
    float desiredMVL;
    float desiredMVR;
    Angle phiGoal;
    float pathGoal;
    RobotPosition lastRobotPos;

    Spline turnSpline = Spline(0, 1, 0.2, 0.2);
    Angle turnBegin;
    Angle turnEnd;

    PathPlanner();
    /*Function Name: void LabTestRun(const RobotPosition &robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the K and forwardVel member variable and trajectory logic
    Modifies: int currentTask;

    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void LabTestRun(const RobotPosition &robotPos);
    /*Function Name: void computeDesiredV();
    Effect: compute desiredMVL and desiredMVR based on the K and forwardVel member variable
    Modifies:
          1. float desiredMVL;
          2. float desiredMVR;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function within the class to compute desiredMVL and desiredMVR
    */
    void computeDesiredV(float forwardVel, float K);
    /*Function Name: void turnToGo1(const RobotPosition & robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y)
    Modifies: 1. int currentTask;
              2. float phiGoal;
              3. float pathGoal;
              4. float xlast;
              5. float ylast;
              6. float philast;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData);

    /*Function Name: void OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) to turn the heading to align with the next point.
    Modifies: 1. float desiredMVL;
              2. float desiredMVR;
              2. float phiGoal;

    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    bool OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData);



  private:
    unsigned long prevSerialTime;
};
