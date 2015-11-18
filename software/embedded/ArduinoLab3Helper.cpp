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

