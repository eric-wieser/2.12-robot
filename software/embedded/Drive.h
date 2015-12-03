#pragma once

#include "DualMC33926MotorShield.h"
#include "LS7366.h"

class Drive {
private:
    DualMC33926MotorShield md;
	LS7366 encA = LS7366(5);
	LS7366 encB = LS7366(6);
public:
	// add sign corrections here. Positive should mean forward
	void setMRSpeed(int s) { md.setM1Speed(s);}
	uint32_t getMREncoder() const { return encA.read(); }

	void setMLSpeed(int s) { md.setM2Speed(-s);}
	uint32_t getMLEncoder() const { return -encB.read(); }

	void resetEncoders() {
		encA.clear();
		encB.clear();
	}

	bool faulted() const {
		return md.getFault();
	}

	bool clearFault() {
		md.clearFault();
	}

	Drive() {
		encA.begin();
		encB.begin();

 		delayMicroseconds(100);

		encA.configure();
 		delayMicroseconds(100);
		encB.configure();

		resetEncoders();
		md.init();
	}
};