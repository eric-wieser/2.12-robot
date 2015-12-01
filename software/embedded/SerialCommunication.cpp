#include "SerialCommunication.h"

#include "RobotPosition.h"

//SerialCommunication Class function implementation
SerialCommunication::SerialCommunication() {
  prevSerialTime = micros();
  finished = true;
  gpsData.received = 0;
}

void SerialCommunication::sendSerialData(const RobotPosition & robotPos) {
  if (micros() - prevSerialTime >= SERIAL_PERIOD_MICROS) {
    Serial.print("P");
    Serial.print(robotPos.pos.x, 6); //X
    Serial.print(",");
    Serial.print(robotPos.pos.y, 6); //Y
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
    char type = commandString[0];
    commandString = commandString.substring(1);

    int i = 0;
    int indexPointer = 0;
    while (indexPointer != -1 ) {
      indexPointer = commandString.indexOf(',');
      tempString = commandString.substring(0, indexPointer);
      commandString = commandString.substring(indexPointer+1);
      command[i] = tempString.toFloat();
      ++i;
    }


    if(type == 'D') {
      commandPos.x = command[0];
      commandPos.y = command[1];
      commandPhi = command[2];
      updateStatus(false);
    }
    else if(type == 'G') {
      gpsData.pos.x = command[0];
      gpsData.pos.y = command[1];
      gpsData.phi = command[2];
      gpsData.received = micros(); // TODO: use the gps timestamp?
    }
    else if (type == 'S') {
      servoGoal = command[0];
    }
    else if (type == 'K') {
      killRequested = true;
    }
    else {
      Serial.print("Unimplemented command type '");
      Serial.print(type);
      Serial.println("'");
      return;
    }
  }
}

void SerialCommunication::updateStatus(boolean currentStatus) {
  finished = currentStatus;
}
