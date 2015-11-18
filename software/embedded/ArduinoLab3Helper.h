/*
2.12 Lab 2: Wheeled Robot Kinematics
Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
Sept. 2015
*/

#ifndef ArduinoLab2Helper_h
#define ArduinoLab2Helper_h

#include "Arduino.h"
#include "encoders.h"
#include "DualMC33926MotorShield.h"


//Some useful constant definitions
#define FREQ 2000
#define PERIOD 0.0005
#define PERIOD_MICROS 500
#define b 0.225
#define r 0.037

//Timing
extern unsigned long currentTime;
extern unsigned long prevTime;
extern unsigned long freqTime;
extern float dt;

#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "PIController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"


#endif


