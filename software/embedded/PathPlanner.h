#pragma once

class RobotPosition;
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
    float K;
    float forwardVel;
    float desiredMVL;
    float desiredMVR;
    float phiGoal;
    float pathGoal;
    float xlast;
    float ylast;
    float philast;
    float pathlast;
    float phiDesired;
    float pathDesired;
    /*Function Name: initialize();
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
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
    void computeDesiredV();
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
     /*Function Name: void turnAndGo(const RobotPosition & robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) to turn and go at the same time
    Modifies: 1. int currentTask;
              2. float phiGoal;          
              3. float xlast;
              4. float ylast;
              5. float philast;
              6. float phiDesired;
              7. float pathDesired;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void turnAndGo(const RobotPosition & robotPos, const SerialCommunication & reportData);
    
    
    /*Function Name: void OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) to turn the heading to align with the next point.
    Modifies: 1. float desiredMVL;
              2. float desiredMVR;        
              2. float phiGoal;
              
    Requirement: None
    Input: None
    Output: whether the goal has been reached
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    bool OrientationController(const RobotPosition & robotPos, const SerialCommunication & reportData, float kp);
 
 
 
  private:
    unsigned long prevSerialTime;
};
