/*
Header File for LS7366 Quadrature Counter
AUTHOR: Daniel J. Gonzlaez - dgonz@mit.edu
DATE: June 2015
*/
#ifndef LS7366_h
#define LS7366_h

//If using Arduino: 

#include "Arduino.h"
#include "SPI.h"


// Slave Select pins for encoders 1 and 2
// Feel free to reallocate these pins to best suit your circuit
const int slaveSelectEnc1 = 5;
const int slaveSelectEnc2 = 6;

//=========================Functions=============================================================


void initEncoders();

void clearEncoderCount();


long readEncoder(int encoder);


#endif
