#pragma once

// include our classes
#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "MotorController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"
#include "Drive.h"
#include "VS11.h"

class Main {
public:
  Main();
  void setup();
  void loop();
private:
  Drive drive;

  EncoderMeasurement measureRobot;
  RobotPosition robotPos;
  MotorController moveRobot;
  SerialCommunication reportData;
  PathPlanner pathPlanner;

  VS11 handServo;

  uint32_t prevTime;
};
