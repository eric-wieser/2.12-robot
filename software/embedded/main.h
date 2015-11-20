#pragma once

// include our classes
#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "MotorController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"

class Main {
public:
  Main();
  void setup();
  void loop();
private:
  EncoderMeasurement measureRobot; //instantiate encoder handler class
  RobotPosition robotPos; //instantiate robot position and orientation calculation class
  MotorController moveRobot; //instantiate velocity PI controller class
  SerialCommunication reportData; //instantiate matlab plot serial communication class
  PathPlanner pathPlanner; //instantiate path planner

  unsigned long currentTime;
  unsigned long prevTime;
};
