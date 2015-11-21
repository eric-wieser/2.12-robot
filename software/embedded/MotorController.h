#pragma once

#include "PIDController.h"

class Drive;

/*
Class Name: PIController
Description: robot motor velocity controller
Uesful public Member Variables: None
Public Member Functions:
    1. void initialize();
    2. void update();
External Function Dependency:
External Variable Dependency:
    1. void doPIControl(String side, float desV, float currV); with side from input string. desV from PathPlanner object and currV from RobotPosition object
*/
class MotorController {
  public:
    MotorController(Drive &d);
    int mRCommand; //reference command to motor 1 range from -400 to 400
    int mLCommand; //reference command to motor 2 range from -400 to 400

    /*
    Effect: Actuate the motor with PI Controller.
    Modifies: All member variables
    Requirement: None
    Input:
        2. float desV; desired velocity in m/s
        3. float currV; current velocity in m/s
    Output: None
    Usage: Call this function to change the velocity of the robot
    */
    void controlMR(float desV, float currV);
    void controlML(float desV, float currV);

  private:
    Drive &drive;
    PIDController mRController = PIDController(18, 2, 0);
    PIDController mLController = PIDController(18, 2, 0);
};
