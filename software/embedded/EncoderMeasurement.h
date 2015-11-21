#pragma once

#include <math.h>
#include <stdint.h>

class Drive;

/*
Class Name: EncoderMeasurement
Description:self contained class for handling encoder measurement
Uesful public Member Variables:
    1. float dThetaL;   //left wheel turned in radians
    2. float dThetaR;   //right wheel turned in radians
Public Member Functions:
    1. void initialize();
    2. void update();
External Function Dependency:  int readEncoder(#); in package #include "LS7366.h"
External Variable Dependency:  None
*/
class EncoderMeasurement {
  public:
    EncoderMeasurement(Drive &drive);

    float dThetaL;   //left wheel turned in radians
    float dThetaR;   //right wheel turned in radians
    float dWheelR;  //increment wheel 1 distance
    float dWheelL;  //increment wheel 2 distance
    float totalWheelR; //total wheel 1 distance
    float totalWheelL;  //total wheel 2 distance
    float mVR; // right wheel velocity in m/s
    float mVL; // left wheel velocity in m/s
    /*
    Function Name: update(float dThetaL, float dThetaR);
    Effect: Update the encoder count and incremental angle of the cycle.
    Modifies: All non-constant member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void update();

  private:
    Drive &drive;

    uint32_t encoderRCount;    //encoder 1 counts in ticks for the current cycle
    uint32_t encoderLCount;   //encoder 2 counts in ticks for the current cycle
    int dEncoderR;    //encoder increment 1
    int dEncoderL;    //encoder increment 2

    const float wheelDiameter = (0.074);  //encoder to wheel, in meters
    const float enc2Wheel = M_PI * wheelDiameter * 12.0 / (20.0 * 637.0); //encoder to wheel, Gearing = 12/637
    const float enc2Theta = 2.0 * M_PI / 20.0 * 12.0 / 637.0; //convert encoder counts to radians
    const float maxMV = 0.41;//22*26.94*wheelDiameter*6.0/637.0;  //22.2 V * 26.94 rad/s/V / (53+1/12) *pi*D
};
