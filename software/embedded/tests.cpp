#include "tests.h"

#include "Drive.h"
#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "MotorController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"
#include "constants.h"

void tests::motor_wiring() {
	Drive md;

	md.resetEncoders();

	{
		Serial.print("Motor R, forward: ");
		uint32_t encr = md.getMREncoder();
		uint32_t encl = md.getMLEncoder();
		Serial.flush();
		md.setMRSpeed(400);
		delay(2e3);
		md.setMRSpeed(0);
		int32_t diffr = md.getMREncoder() - encr;
		int32_t diffl = md.getMLEncoder() - encl;
		Serial.print("dr =");
		Serial.print(diffr);
		Serial.print(", dl =");
		Serial.println(diffl);
	}
	{
		Serial.print("Motor L, forward: ");
		uint32_t encr = md.getMREncoder();
		uint32_t encl = md.getMLEncoder();
		Serial.flush();
		md.setMLSpeed(400);
		delay(2e3);
		md.setMLSpeed(0);
		int32_t diffr = md.getMREncoder() - encr;
		int32_t diffl = md.getMLEncoder() - encl;
		Serial.print("dr =");
		Serial.print(diffr);
		Serial.print(", dl =");
		Serial.println(diffl);
	}
}

void tests::encoder_wiring() {
	Drive md;

	while(true) {
		uint32_t encr = md.getMREncoder();
		uint32_t encl = md.getMLEncoder();
		Serial.print(encr);
		Serial.print(", ");
		Serial.print(encl);
		Serial.println();
		delay(10);
	}
}

void tests::measurements() {
	Drive md;
	EncoderMeasurement measure(md);

	md.setMLSpeed(400);
	md.setMRSpeed(-400);

	uint32_t prevTime;
	int i =0;
	while(true) {
		uint32_t currTime = micros();
		if (currTime - prevTime >= PERIOD_MICROS) {
			prevTime = currTime;
			measure.update();
			i++;
			if(i % 5 == 0) {
				Serial.print(prevTime);
				Serial.print("\t");
				Serial.print(measure.mVR);
				Serial.print("\t");
				Serial.println(measure.mVL);
			}
		}
	}
}


void tests::motor_feedback() {
	Drive md;

	MotorController cont(md);
	EncoderMeasurement measure(md);

	uint32_t prevTime;
	int i =0;
	while(true) {
		uint32_t currTime = micros();
		if (currTime - prevTime >= PERIOD_MICROS) {
			prevTime = currTime;
			measure.update();

			cont.controlMR(1, measure.mVR); // right motor PI control
			cont.controlML(-1, measure.mVL); //left motor PI control

			i++;
			if(i % 10 == 0) {
				Serial.print(measure.mVR);
				Serial.print(", ");
				Serial.println(measure.mVL);
			}
			if(md.faulted()) {
				Serial.println("Motor fault");
				md.setMLSpeed(0);
				md.setMRSpeed(0);
				delay(100);
				md.clearFault();
			}
		}
	}

}