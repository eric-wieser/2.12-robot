#include "EncoderMeasurement.h"

#include "encoders.h"
#include "constants.h"

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
}
