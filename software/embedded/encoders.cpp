#include "encoders.h"

using namespace encoders;

LS7366 encoders::left(5);
LS7366 encoders::right(6);

void initEncoders() {
  left.begin();
  right.begin();

  left.configure();
  right.configure();
}


void clearEncoderCount() {
  left.clear();
  right.clear();
}


long readEncoder(int encoder) {
  if (encoder == 1) {
    return left.read();
  }

  else if (encoder == 2) {
    return right.read();
  }

  return 0;
}
