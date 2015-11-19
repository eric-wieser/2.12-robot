#pragma once

#include "DualMC33926MotorShield.h"

enum Side {
    SIDE_LEFT = 2,
    SIDE_RIGHT = 1
};

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
class PIController {
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
    Function Name: void doPIControl(String side, float refV, float currV);
    Effect: Actuate the motor with PI Controller.
    Modifies: All member variables
    Requirement: None
    Input:
        1. String side; use "Left" or "Right" strings for implementation
        2. float desV; desired velocity in m/s
        3. float currV; current velocity in m/s
    Output: None
    Usage: Call this function to change the velocity of the robot
    */
    void doPIControl(Side side, float desV, float currV);

  private:
    float integratedVError1; //integrator error for motor 1
    float integratedVError2; //integrator error for motor 2
    float integralCommand1; //integrated command for motor 1
    float integralCommand2; // integrated command for motor 2
    const int Kpv1 = 18; //proportional gain for motor 1
    const int Kpv2 = 18; //proportional gain for motor 2
    const int Kdv1 = 0; //derivative gain for motor 1
    const int Kdv2 = 0; //derivative gain for motor 2
    const float Kiv1 = 2.0; //integrator gain for motor 1
    const float Kiv2 = 2.0; //integrator gain for motor 2
    int PrevCommand1; //previous command for motor 1
    int PrevCommand2; //previous command for motor 2
};
