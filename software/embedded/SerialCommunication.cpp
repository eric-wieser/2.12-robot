#include "SerialCommunication.h"

#include "RobotPosition.h"

//SerialCommunication Class function implementation
void SerialCommunication::initialize() {
  prevSerialTime = micros();
}

void SerialCommunication::sendSerialData(const RobotPosition & robotPos) {
  if (micros() - prevSerialTime >= SERIAL_PERIOD_MICROS) {
    Serial.print(robotPos.X, 6); //X
    Serial.print(",");
    Serial.print(robotPos.Y, 6); //Y
    Serial.print(",");
    Serial.print(float(robotPos.Phi)); //Phi
    Serial.print(",");
    Serial.println(finished ? 1 : 0);
    prevSerialTime = micros();
  }
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
}

void SerialCommunication::updateStatus(boolean currentStatus) {
  finished = currentStatus;
}
