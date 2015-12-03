/**
This is the program run by the arduino
We can largely ignore loop(), because it's basically equivalent to sticking
a while loop in setup()

To see exactly how this works, see $ARDUINO_INSTALL_DIR\hardware\arduino\avr\cores\arduino\main.cpp
*/
#include "main.h"
#include "hacks.h"
#include "tests.h"

char buffer[sizeof(Main)];
Main * m;

void setup() {
  Serial.begin(115200);      // Initialize Serial Communication
  Serial.println("Starting up");
  // m = new(buffer) Main();

  // tests::motor_feedback();
  // tests::motor_wiring();
  tests::servo_drive_interaction();

  Serial.println("done");
  Serial.flush();
}

void loop() {
  // m->loop();
}






