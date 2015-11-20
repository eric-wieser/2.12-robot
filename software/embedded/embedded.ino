/*
2.12 Lab 2: Wheeled Robot Kinematics

Zack Bright - zbright@mit.edu
& Dan Gonzalez - dgonz@mit.edu
& Fangzhou Xia- xiafz@mit.edu
Sept. 2015
*/

#include "main.h"
#include "hacks.h"

// char buffer[sizeof(Main)];
char buffer[sizeof(Robot)];
Main * m;

void setup() {
  Serial.begin(115200);      // Initialize Serial Communication
  Serial.println("Starting up");
  m = new(buffer) Main();
  Serial.println("done");
  Serial.flush();
}

void loop() {
  m->loop();
}






