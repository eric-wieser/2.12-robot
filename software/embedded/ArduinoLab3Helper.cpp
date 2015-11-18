/*
2.12 Lab 2: Wheeled Robot Kinematics
Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
& Fangzhou Xia - xiafz@mit.edu
Sept. 2015
*/

#include <math.h>
#include "ArduinoLab3Helper.h"

//Timing
unsigned long currentTime;
unsigned long prevTime = 0;
unsigned long freqTime = 0;
unsigned long currentSerialTime = 0;
unsigned long prevSerialTime = 0;

//Encoder Measurement Class function implementation
void EncoderMeasurement::initialize() {
  encoder1Count = 0;
  encoder2Count = 0;
  encoder1CountPrev = 0;
  encoder2CountPrev = 0;
  dEncoder1 = 0;
  dEncoder2 = 0;
  dTheta1 = 0.0;
  dTheta2 = 0.0;
  dThetaR = 0.0;
  dThetaL = 0.0;
  dWheel1 = 0.0;
  dWheel2 = 0.0;
  totalWheel1 = 0.0;
  totalWheel2 = 0.0;
  mV1 = 0.0;
  mV2 = 0.0;
  return;
}

void EncoderMeasurement::update() {
  encoder1Count = readEncoder(1);
  encoder2Count = -1 * readEncoder(2);
  dEncoder1 = (encoder1Count - encoder1CountPrev);
  dEncoder2 = (encoder2Count - encoder2CountPrev);
  //update the angle incremet in radians
  dTheta1 = (dEncoder1 * enc2Theta);
  dTheta2 = (dEncoder2 * enc2Theta);
  //for encoder index and motor position switching (Left is 2, Right is 1)
  dThetaR = dTheta1;
  dThetaL = dTheta2;
  // update the wheel distance travelled in meters
  dWheel1 = (dEncoder1 * enc2Wheel);
  dWheel2 = (dEncoder2 * enc2Wheel);
  totalWheel1 = encoder1Count * enc2Wheel;
  totalWheel2 = encoder2Count * enc2Wheel;;
  //motor velocity (Left is 2, Right is 1)
  mV1 = dWheel1 / PERIOD;
  mV2 = dWheel2 / PERIOD;
  mVL = mV2;
  mVR = mV1;
  encoder1CountPrev = encoder1Count;
  encoder2CountPrev = encoder2Count;
  return;
}

//RobotPosition Class function implementation
void RobotPosition::initialize() {
  dPhi = 0;
  Phi = 0;
  prevPhi = 0;
  AvgPhi = 0;
  dX = 0;
  dY = 0;
  X = 0;
  Y = 0;
  prevX = 0;
  prevY = 0;
  pathDistance = 0;
  return;
}

void RobotPosition::update(float dThetaL, float dThetaR) {
  dPhi = (r / (2.0 * b)) * (dThetaR - dThetaL);
  Phi = prevPhi + dPhi;
  AvgPhi = (Phi + prevPhi) / 2.0;
  prevPhi = Phi;
  dX = (r / 2.0) * cos(AvgPhi) * (dThetaR + dThetaL);
  X = prevX + dX;
  dY = (r / 2.0) * sin(AvgPhi) * (dThetaR + dThetaL);
  Y = prevY + dY;
  prevX = X;
  prevY = Y;
  pathDistance  += sqrt(dX * dX + dY * dY);
  return;
}

//PIController Class function implementation
void PIController::initialize() {
  md.init();  
  m1Command = 0;
  m2Command = 0;
  PrevCommand1 = 0;
  PrevCommand2 = 0;
  integratedVError1 = 0;
  integratedVError2 = 0;
  integralCommand1 = 0;
  integralCommand2 = 0;
  return;
}

void PIController::doPIControl(String side, float desV, float currV) {
  desV = constrain(desV,-0.41, 0.41);
  if (side == "Right") {
    //Motor 1 velocity control
    integratedVError1 = integratedVError1 + (desV - currV) * PERIOD;
    integralCommand1 = constrain(Kiv1 * integratedVError1, -200, 200);
    //integralCommand1 = Kiv1*integratedVError1;
    //derivativeVError1 = ((velocity - desiredMV1Prev) - (mV1 - mV1Prev))/dt;
    //derivativeCommand1 = Kdv1*derivativeVError1;
    m1Command = Kpv1 * (desV - currV) + Kdv1 * (0) + integralCommand1;
    m1Command += PrevCommand1;
    md.setM1Speed(constrain(m1Command, -400, 400));
    PrevCommand1 = m1Command;
  }
  else if (side == "Left") {
    //Motor 2 velocity control
    integratedVError2 = integratedVError2 + (desV - currV) * PERIOD;
    integralCommand2 = constrain(Kiv2 * integratedVError2, -200, 200);
    m2Command = Kpv2 * (desV - currV) + Kdv2 * (0) + integralCommand2;
    m2Command += PrevCommand2;
    md.setM2Speed(constrain(-1 * m2Command, -400, 400));
    PrevCommand2 = m2Command;
  }
  else {
    md.setM1Speed(0);
    md.setM2Speed(0);
    Serial.println("ERROR: INVALID MOTOR CHOICE GIVEN TO PI CONTROLLER");
  }
  return;
}

//SerialCommunication Class function implementation
void SerialCommunication::initialize() {
  prevSerialTime = micros();
  return;
}

void SerialCommunication::sendSerialData(const RobotPosition & robotPos) {
  if (micros() - prevSerialTime >= SERIAL_PERIOD_MICROS) {
    Serial.print(robotPos.X, 6); //X
    Serial.print(",");
    Serial.print(robotPos.Y, 6); //Y
    Serial.print(",");
    Serial.print(robotPos.Phi); //Phi
    Serial.print(",");
    Serial.println(finished ? 1 : 0);
    prevSerialTime = micros();
  }
  return;
}

void SerialCommunication::receiveSerialData() {
    if (Serial.available() > 0) {
      commandString = Serial.readString();
      int i = 0;
      indexPointer = 0;
      while (indexPointer != -1 ) {
        indexPointer = commandString.indexOf(',');
        tempString = commandString.substring(0, indexPointer);
        commandString = commandString.substring(indexPointer+1);
        command[i] = tempString.toFloat();
        ++i;
      }
      commandX = command[0];
      commandY = command[1];
      commandPhi = command[2];
      updateStatus(false);
    }
    return;
  }

void SerialCommunication::updateStatus(boolean currentStatus) {
  finished = currentStatus;
  return;
}

//PathPlanner Class function implementation
void PathPlanner::initialize() {
  currentTask = 1;
  K = 0;
  forwardVel = 0;
  desiredMVL = 0;
  desiredMVR = 0;
  phiGoal = 0;
  xlast = 0;
  ylast = 0;
  philast = 0;
  pathlast = 0;
  phiDesired = 0;
  pathDesired = 0;
  return;
}

void PathPlanner::LabTestRun(const RobotPosition & robotPos) {
  if (currentTask == 1) { //we are moving forward
    if (robotPos.pathDistance < 1.0) { //if we aren't there yet, keep going
      forwardVel = .2;
      K = 0;
      computeDesiredV();

    } else { //if we are there, stop, reset distances, and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 2;
    }
  }
  if (currentTask == 2) { //we are turning around
    if (robotPos.pathDistance < 1.0 + (PI * .25)) { //if we aren't there yet, keep going
      forwardVel = .1;
      K = 4;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 3;
    }
  }
  if (currentTask == 3) { //we are going back home
    if (robotPos.pathDistance < 2.0 + (PI * .25) ) { //if we aren't there yet, keep going
      forwardVel = .2;
      K = 0;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      currentTask = 4;
      Serial.println("DONE MOVING");
    }
  }
  return;
}

void PathPlanner::computeDesiredV() {
  //command velocities based off of K and average forwardVel
  desiredMVR = forwardVel * (1 + K * b);
  desiredMVL = forwardVel * (1 - K * b);
  return;
}

void PathPlanner::OrientationController(const RobotPosition & robotPos, SerialCommunication & reportData) {
  float eps = .01;
  float KPhi = .50;
  float delY = reportData.commandY - robotPos.Y;
  float delX = reportData.commandX - robotPos.X;
  
  phiGoal = atan2((delY), (delX));
  float currentPhiError = fmod(phiGoal - robotPos.Phi + PI, 2*PI) - PI;
  if (currentPhiError > PI/3 || currentPhiError < PI/18){
    KPhi = 10.0;
  }
  
   desiredMVR += KPhi * currentPhiError*2.0*b;
   desiredMVL -= KPhi * currentPhiError*2.0*b;

   if( abs(delX) < eps && abs(delY) < eps) {
    reportData.updateStatus(true);
   }
  return;
}

void PathPlanner::turnToGo(const RobotPosition & robotPos, SerialCommunication & reportData) {
  //take next point (X,Y) positions given by MatLab, calculate phi to turn towards, then go straight 
  phiGoal = atan2((reportData.commandY - ylast), (reportData.commandX - xlast));
  //pathGoal = sqrt((reportData.commandX - xlast) * (reportData.commandX - xlast) + (reportData.commandY - ylast) * (reportData.commandY - ylast)) + pathlast;
  // constrain the error in phi to the [-pi, pi)
  float lastPhiError = fmod(phiGoal - philast + PI, 2*PI) - PI;
  float currentPhiError = fmod(phiGoal - robotPos.Phi + PI, 2*PI) - PI;
  if (currentTask == 0) { // waiting to receive command x,y
    desiredMVR = 0;
    desiredMVL = 0;
    if (!reportData.finished){
      currentTask = 1;
    }
  }
  if (currentTask == 1) { //turn towards next point 
    if (lastPhiError > 0 || (lastPhiError <0 && 2*PI-abs(lastPhiError)<PI)){ //turn counter clock wise
      if (currentPhiError >= 0){
        desiredMVR = 0.2;
        desiredMVL = -0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        float pathAfterTurn = robotPos.pathDistance;
        pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));

      }
    }  
    if (lastPhiError < 0 || (lastPhiError > 0 && 2*PI-abs(lastPhiError)<PI)){ //turn clock wise
      if (currentPhiError <= 0){
        desiredMVR = -0.2;
        desiredMVL = 0.2;
      } else {
        desiredMVR = 0;
        desiredMVL = 0;
        currentTask = 2;
        float pathAfterTurn = robotPos.pathDistance;
        pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));

      }
    }
    if (lastPhiError == 0) { //don't need to turn
      currentTask = 2;
      float pathAfterTurn = robotPos.pathDistance;
      pathGoal = pathAfterTurn+sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y));
    }
  }
  
  if (currentTask == 2) { //now go straight to next point
    if (robotPos.pathDistance < pathGoal ) { //if we aren't there yet, keep going
      forwardVel = .2;
      computeDesiredV();
      OrientationController(robotPos, reportData);
    } else { //if we are there, stop and move on to the next task
      forwardVel = 0;
      computeDesiredV();
      currentTask = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
      reportData.updateStatus(true);
    }
  }
  return;
}

void PathPlanner::turnAndGo(const RobotPosition & robotPos, const SerialCommunication & reportData) {
  //take next point (X,Y) positions given by MatLab, calculate phi to turn towards, then go forward and turn at the same time 
  phiGoal = tan((reportData.commandY - ylast) / (reportData.commandX - xlast));
  
  if (philast > phiGoal){ //turn clock wise 
    forwardVel = .2;
    phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));
    
    if (robotPos.Phi > phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      K = -4;
      computeDesiredV();
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight 
      K=0;
      computeDesiredV();
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
    }  
  } else { //turn counter clock wise
      forwardVel = .2; 
      phiDesired = tan((reportData.commandY - robotPos.Y) / (reportData.commandX - robotPos.X));
    if (robotPos.Phi < phiDesired) { //if we are not pointing directly at the way point, keep going and turning
      K = 4;
      computeDesiredV();
      pathDesired = sqrt((reportData.commandX - robotPos.X) * (reportData.commandX - robotPos.X) + (reportData.commandY - robotPos.Y) * (reportData.commandY - robotPos.Y))+robotPos.pathDistance;
    } else if (robotPos.pathDistance < pathDesired) { //if we are pointing toward the way point but not there yet, keep going straight 
      K=0;
      computeDesiredV();      
    } else { //if we are there, stop and move on to the next task
      desiredMVR = 0;
      desiredMVL = 0;
      xlast = robotPos.X;
      ylast = robotPos.Y;
      philast = robotPos.Phi;
      pathlast = robotPos.pathDistance;
      Serial.println("NEXT POINT");
    } 
  }
  
  return;
}

// checks if an angle (in radians) is in the neighborhood of angletarget
// even if those angles are not expressed in the range [0,2*pi)
boolean angleInNeighborhood(float angle, float angleTarget, float margin) {
  // set the angles to be constrained to the range [0,2*pi)
  float angleConstrained = fmod(angle, 2*PI);
  float angleTargetConstrained = fmod(angleTarget, 2*PI);
  // check if the constrained angle is in the neighborhood
  // or the neighborhood +2pi, or the neighborhood -2pi
  if ((angleTarget - 2*PI - margin) < angleConstrained && angleConstrained < (angleTarget - 2*PI + margin)) {
    return true;
  }
  if ((angleTarget - margin) < angleConstrained && angleConstrained < (angleTarget + margin)) {
    return true;
  }
  if ((angleTarget + 2*PI - margin) < angleConstrained && angleConstrained < (angleTarget + 2*PI + margin)) {
    return true;
  }
  // otherwise, it's not in the range neighborhood
  return false;
}

