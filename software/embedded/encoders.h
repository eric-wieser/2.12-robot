#ifndef ENCODERS_h
#define ENCODERS_h

#include "LS7366.h"

namespace encoders {
  extern LS7366 left;
  extern LS7366 right;
}

void initEncoders();
void clearEncoderCount();
long readEncoder(int encoder);

#endif