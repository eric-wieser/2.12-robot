/*
Main File for LS7366 Quadrature Counter
AUTHOR: Daniel J. Gonzlaez - dgonz@mit.edu
DATE: June 2015
*/

//=========================HEADER=============================================================
/*

   Hardware: Arduino Uno R3
   Powered
   `
   LS7366 Breakout    -------------   Arduino
   -----------------                    -------
            MOSI   -------------------   SDO (D11)
            MISO   -------------------   SDI (D12)
            SCK    -------------------   SCK (D13)
            SS1    -------------------   SS1 (D6)
            SS2    -------------------   SS2 (D8)
            GND    -------------------   GND
            VDD    -------------------   VCC (5.0V)

   License: CCAv3.0 Attribution-ShareAlike (http://creativecommons.org/licenses/by-sa/3.0/)
   You're free to use this code for any venture. Attribution is greatly appreciated.

//============================================================================================
*/

//If using Arduino:
#include "LS7366.h"
#include "Arduino.h"
#include "SPI.h"

// Instruction register contants, from the datasheet
static const int MDR0 = 1 << 3,
                 MDR1 = 2 << 3,
                 DTR  = 3 << 3,
                 CNTR = 4 << 3,
                 OTR  = 5 << 3,
                 STR  = 6 << 3;
static const int CLR  = 0 << 6,
                 RD   = 1 << 6,
                 WR   = 2 << 6,
                 LOAD = 3 << 6;

LS7366::LS7366(int pin) : cs_pin(pin) { }

void LS7366::begin() {
  // Set slave select as outputs
  pinMode(cs_pin, OUTPUT);

  // Raise select pin
  // Communication begins when you drop the signal
  digitalWrite(cs_pin, HIGH);

  SPI.begin();
}

void LS7366::configure() {
  digitalWrite(cs_pin,LOW);        // Begin SPI conversation
  SPI.transfer(WR | MDR0);         // Write to MDR0
  SPI.transfer(0x03);              // Configure to x4  mode
  digitalWrite(cs_pin,HIGH);       // Terminate SPI conversation
}

uint32_t LS7366::read() const {
  uint32_t result = 0;

  digitalWrite(cs_pin, LOW);      // Begin SPI conversation
  SPI.transfer(RD | CNTR);        // Request count
  result |= static_cast<uint32_t>(SPI.transfer(0x00)) << 24;   // Read highest order byte
  result |= static_cast<uint32_t>(SPI.transfer(0x00)) << 16;
  result |= static_cast<uint32_t>(SPI.transfer(0x00)) << 8;
  result |= static_cast<uint32_t>(SPI.transfer(0x00));         // Read lowest order byte
  digitalWrite(cs_pin, HIGH);     // Terminate SPI conversation

  return result;
}

void LS7366::clear() {
  // Set data register to 0
  digitalWrite(cs_pin, LOW);      // Begin SPI conversation
  SPI.transfer(WR | DTR);
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(cs_pin, HIGH);     // Terminate SPI conversation

  delayMicroseconds(100);  // provides some breathing room between SPI conversations

  // Set current data register to center
  digitalWrite(cs_pin,LOW);      // Begin SPI conversation
  SPI.transfer(LOAD | CNTR);
  digitalWrite(cs_pin,HIGH);     // Terminate SPI conversation
}
