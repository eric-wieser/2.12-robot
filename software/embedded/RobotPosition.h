#pragma once

#include "Angle.h"

/*
Class Name: RobotPosition
Description: vehicle position and orientation calculator
Uesful public Member Variables:
    1. float X;  // robot X position in meters
    2. float Y;  // robot Y position in meters
    3. float Phi; // robot global orientation in meters
Public Member Functions:
    1. void initialize();
    2. void update(float dThetaL, float dThetaR); //update the value
External Function Dependency:  None
External Variable Dependency: float dThetaL, float dThetaR from object of EncoderMeasurement
*/
class RobotPosition {
  public:
    float X;  //robot X position in meters
    float Y;  //robot Y position in meters
    Angle Phi; //robot global orientation in meters
    float pathDistance; //trajectory path distance in meters
    /*
    Function Name: initialize()
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    RobotPosition();
    /*
    Function Name: update(float dThetaL, float dThetaR);
    Effect: Update the X,Y position and vehicle orientation angle Phi
    Modifies: All member variables
    Requirement: None
    Input:
        1. float dThetaL; left wheel increment angle in radians
        2. float dThetaR; right wheel increment angle in radians
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void update(float dThetaL, float dThetaR); //update the value
};