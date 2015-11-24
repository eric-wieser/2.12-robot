#pragma once

// needed for String
#include "Arduino.h"
#include "Vector.h"

// forward declaration
class RobotPosition;

#define SERIAL_FREQ 100
#define SERIAL_PERIOD 0.01
#define SERIAL_PERIOD_MICROS 10000


/*
Class Name: SerialCommunication
Description: Timed serial communication for data reporting to Matlab
Uesful public Member Variables: None
Public Member Functions:
    1. void initialize();
    2. void doSerialCommunication(const RobotPosition & robotPos);
External Function Dependency:  micro()
External Variable Dependency:
    1. void doSerialCommunication(const RobotPosition & robotPos); with RobotPosition type object constant input passed by reference
*/
class SerialCommunication {
  public:
    float command[10];
    Vector commandPos;
    float commandPhi;
    bool finished;

    struct GPSReport {
        Vector pos;
        Angle phi;
        uint32_t received;
    };

    GPSReport gpsData;

    float servoGoal;

    bool killRequested = false;

    SerialCommunication();
    /*Function Name: sendSerialData(const RobotPosition & robotPos);
    Effect: report serial data to matlab
    Modifies: None
    Requirement: None
    Input: const RobotPosition & robotPos
    Output: None
    Usage; Call this function to check and receive serial data in each timed loop
    */
    void sendSerialData(const RobotPosition & robotPos);
    /*Function Name: receiveSerialData();
    Effect: Initialize all public member variables of the class to 0
    Modifies:
          1. float command[10];
          2. float commandX;
          3. float commandY;
          4. float commandPhi;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function to send serial data in each timed loop
    */
    void receiveSerialData();
    /*Function Name: updateStatus(bool finished);
    Effect: change the private member variable bool finished
    Modifies: private member variable bool finished
    Requirement: None
    Input: bool status
    Output: None
    Usage; Call this function to update the status to send
    */
    void updateStatus(bool currentStatus);

  private:
    unsigned long prevSerialTime;
    String commandString;
    String tempString;
};