/*
2.12 Lab 2: Wheeled Robot Kinematics

Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
& Fangzhou Xia- xiafz@mit.edu
Sept. 2015
*/


#include "Arduino.h"

// include our classes
#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "PIController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"

#include "constants.h"

EncoderMeasurement measureRobot; //instantiate encoder handler class
RobotPosition robotPos; //instantiate robot position and orientation calculation class
PIController moveRobot; //instantiate velocity PI controller class
SerialCommunication reportData; //instantiate matlab plot serial communication class
PathPlanner pathPlanner; //instantiate path planner

unsigned long currentTime;
unsigned long prevTime;

void setup() {
  Serial.begin(115200);      // Initialize Serial Communication
 
  currentTime = micros(); //initialize timer
  prevTime = micros(); //initialize timer
  measureRobot.initialize(); //initialize robot encoder handler
  robotPos.initialize();//initialize robot position and orientation calculation
  moveRobot.initialize(); Serial.println("Motor Driver Initialized...");//intialize velocity PI controller
  reportData.initialize(); //initialize matlab plot serial communication
  pathPlanner.initialize(); //initialize path planner
  delay(1e3);// set a delay so the robot doesn't drive off without you

}

void loop() {

  //timed loop implementation
  if (micros() - prevTime >= PERIOD_MICROS) {
    currentTime = micros();
    measureRobot.update(); //check encoder
    robotPos.update(measureRobot.dThetaL, measureRobot.dThetaR); //update position

    reportData.sendSerialData(robotPos); //report data to matlab via serial communication
    reportData.receiveSerialData();
    //pathPlanner.LabTestRun(robotPos); //plan the next path

    pathPlanner.turnToGo(robotPos, reportData); //turn then go straight towards next point

    /*
    if (!reportData.finished){
    pathPlanner.desiredMVL = .2;
    pathPlanner.desiredMVR = .2;
    }

    pathPlanner.OrientationController(robotPos, reportData);
    if (reportData.finished){
      pathPlanner.desiredMVL = 0.0;
    pathPlanner.desiredMVR = 0.0;
    }
    */

    moveRobot.doPIControl(SIDE_LEFT, pathPlanner.desiredMVL, measureRobot.mVL); //left motor PI control
    moveRobot.doPIControl(SIDE_RIGHT, pathPlanner.desiredMVR, measureRobot.mVR); // right motor PI control
    prevTime = currentTime; //update timer
  }
}





