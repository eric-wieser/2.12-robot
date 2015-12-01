/*
2.12 Lab 2: Wheeled Robot Kinematics

Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
& Fangzhou Xia- xiafz@mit.edu
Sept. 2015
*/


#include "main.h"

#include "Arduino.h"
#include "constants.h"

Main::Main() : measureRobot(drive), moveRobot(drive) {
  prevTime = micros();
  delay(1e3);// set a delay so the robot doesn't drive off without you
}

void Main::loop() {

  //timed loop implementation
  uint32_t currentTime = micros();
  if (currentTime - prevTime >= PERIOD_MICROS) {
    measureRobot.update(); //check encoder
    robotPos.update(measureRobot.dThetaL, measureRobot.dThetaR); //update position

    if(reportData.gpsData.received > prevTime) {
      robotPos.pos = reportData.gpsData.pos;
      robotPos.Phi = reportData.gpsData.phi;
      Serial.print('Updated from the GPS data');
    }

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

    moveRobot.controlMR(pathPlanner.desiredMVR, measureRobot.mVR); // right motor PI control
    moveRobot.controlML(pathPlanner.desiredMVL, measureRobot.mVL); //left motor PI control
    prevTime = currentTime; //update timer

    if(reportData.killRequested) {
      drive.setMRSpeed(0);
      drive.setMLSpeed(0);
      Serial.println("Killed by master");

      while(1);
    }
  }
}




