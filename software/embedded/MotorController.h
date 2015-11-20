#pragma once

#include "DualMC33926MotorShield.h"

#include "PIDController.h"

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
    DualMC33926MotorShield md;
    int m1Command; //reference command to motor 1 range from -400 to 400
    int m2Command; //reference command to motor 2 range from -400 to 400
    /*Function Name: initialize()
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
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
    void controlM1(float desV, float currV);
    void controlM2(float desV, float currV);

  private:
    PIDController m1Controller = PIDController(18, 2, 0);
    PIDController m2Controller = PIDController(18, 2, 0);
};
