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
#define PI 3.14159265
#define FREQ 2000
#define PERIOD 0.0005
#define PERIOD_MICROS 500
#define SERIAL_FREQ 100
#define SERIAL_PERIOD 0.01
#define SERIAL_PERIOD_MICROS 10000
#define b 0.225
#define r 0.037

//Timing
extern unsigned long currentTime;
extern unsigned long prevTime;
extern unsigned long freqTime;
extern unsigned long currentSerialTime;
extern unsigned long prevSerialTime;
extern float dt;

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
    float dThetaL;   //left wheel turned in radians
    float dThetaR;   //right wheel turned in radians
    signed long encoder1Count;    //encoder 1 counts in ticks for the current cycle
    signed long encoder2Count;   //encoder 2 counts in ticks for the current cycle
    signed long encoder1CountPrev;   //encoder 1 counts in ticks for the previous cycle
    signed long encoder2CountPrev;   //encoder 2 counts in ticks for the previous cycle
    int dEncoder1;    //encoder increment 1
    int dEncoder2;    //encoder increment 2
    float dWheel1;  //increment wheel 1 distance
    float dWheel2;  //increment wheel 2 distance
    float totalWheel1; //total wheel 1 distance
    float totalWheel2;  //total wheel 2 distance
    float mVL; // left wheel velocity in m/s
    float mVR; // right wheel velocity in m/s
    /*
    Function Name: initialize()
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
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
    float dTheta1;  //wheel 1 turned in radians
    float dTheta2;  //wheel 2 wheel turned in radians
    float mV1; //wheel 1 velocity in m/s
    float mV2; //wheel 2 velocity in m/s
    const float wheelDiameter = (0.074);  //encoder to wheel, in meters
    const float enc2Wheel = PI * wheelDiameter * 12.0 / (20.0 * 637.0); //encoder to wheel, Gearing = 12/637
    const float enc2Theta = 2.0 * PI / 20.0 * 12.0 / 637.0; //convert encoder counts to radians
    const float maxMV = 0.41;//22*26.94*wheelDiameter*6.0/637.0;  //22.2 V * 26.94 rad/s/V / (53+1/12) *pi*D
};

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
    float Phi; //robot global orientation in meters
    float pathDistance; //trajectory path distance in meters
    float AvgPhi; //average orientation angle phi between to timed loop iterations in radians
    /*
    Function Name: initialize()
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
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

  private:
    float dPhi; //orientation angle phi increment in radians
    float prevPhi; //previous orientation angle phi value in radians
    float dX;  //robot X position increment in meters
    float dY; //robot Y position increment in meters
    float prevX; //previous robot X position
    float prevY; //previous robot Y position
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
    void doPIControl(String side, float desV, float currV);

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
    float commandX;
    float commandY;
    float commandPhi;
    boolean finished;
    /*Function Name: initialize();
    Effect: Initialize all public member variables of the class to 0 and prevSerialTime to micro()
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
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
    /*Function Name: updateStatus(boolean finished);
    Effect: change the private member variable boolean finished
    Modifies: private member variable boolean finished
    Requirement: None
    Input: boolean status
    Output: None
    Usage; Call this function to update the status to send
    */
    void updateStatus(boolean currentStatus);
    
  private:
    unsigned long prevSerialTime;
    String commandString;
    String tempString;
    int indexPointer = 0;
};

/*
Class Name: PathPlanner
Description: Timed serial communication for data reporting to Matlab
Uesful public Member Variables: None
Public Member Functions:
    1. void initialize();
    2. void navigate(const RobotPosition &robotPos);
    3. void computeDesiredV();
External Function Dependency: None
External Variable Dependency:
    1.  void navigate(const RobotPosition &robotPos); with RobotPosition type object constant input passed by reference
*/
class PathPlanner {
  public:
    int currentTask;
    float K;
    float forwardVel;
    float desiredMVL;
    float desiredMVR;
    float phiGoal;
    float pathGoal;
    float xlast;
    float ylast;
    float philast;
    float pathlast;
    float phiDesired;
    float pathDesired;
    /*Function Name: initialize();
    Effect: Initialize all public member variables of the class to 0
    Modifies: All member variables
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function upon instantiation of the class to a global object
    */
    void initialize();
    /*Function Name: void LabTestRun(const RobotPosition &robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the K and forwardVel member variable and trajectory logic
    Modifies: int currentTask;

    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void LabTestRun(const RobotPosition &robotPos);
    /*Function Name: void computeDesiredV();
    Effect: compute desiredMVL and desiredMVR based on the K and forwardVel member variable
    Modifies:
          1. float desiredMVL;
          2. float desiredMVR;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function within the class to compute desiredMVL and desiredMVR
    */
    void computeDesiredV();
    /*Function Name: void turnToGo1(const RobotPosition & robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) 
    Modifies: 1. int currentTask;
              2. float phiGoal;
              3. float pathGoal;
              4. float xlast;
              5. float ylast;
              6. float philast;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData);
     /*Function Name: void turnAndGo(const RobotPosition & robotPos);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) to turn and go at the same time
    Modifies: 1. int currentTask;
              2. float phiGoal;          
              3. float xlast;
              4. float ylast;
              5. float philast;
              6. float phiDesired;
              7. float pathDesired;
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void turnAndGo(const RobotPosition & robotPos, const SerialCommunication & reportData);
    
    
    /*Function Name: void OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData);
    Effect: modify the member variable desiredMVL and desiredMVR based on the next goal position (X,Y) to turn the heading to align with the next point.
    Modifies: 1. float desiredMVL;
              2. float desiredMVR;        
              2. float phiGoal;
              
    Requirement: None
    Input: None
    Output: None
    Usage; Call this function in each timed loop to update the desired wheel velocity
    */
    void OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData);
 
 
 
  private:
    unsigned long prevSerialTime;
};

#endif


