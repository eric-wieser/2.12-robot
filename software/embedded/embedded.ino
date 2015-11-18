/*
2.12 Lab 2: Wheeled Robot Kinematics

Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
& Fangzhou Xia- xiafz@mit.edu
Sept. 2015
*/


//Include the SPI bus library, the encoder buffer library, and the motor shield library
#include "Arduino.h"
#include "SPI.h"
#include "LS7366.h"
#include "DualMC33926MotorShield.h"
#include "ArduinoLab3Helper.h"

EncoderMeasurement measureRobot; //instantiate encoder handler class
RobotPosition robotPos; //instantiate robot position and orientation calculation class
PIController moveRobot; //instantiate velocity PI controller class
SerialCommunication reportData; //instantiate matlab plot serial communication class
PathPlanner pathPlanner; //instantiate path planner




void setup() {
  Serial.begin(115200);      // Initialize Serial Communication
  initEncoders();       Serial.println("Encoders Initialized..."); //initialize Encoders
  clearEncoderCount();  Serial.println("Encoders Cleared..."); //clear Encoder Counts
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
    
    moveRobot.doPIControl("Left", pathPlanner.desiredMVL, measureRobot.mVL); //left motor PI control
    moveRobot.doPIControl("Right", pathPlanner.desiredMVR, measureRobot.mVR); // right motor PI control
    prevTime = currentTime; //update timer
    
  }
}




