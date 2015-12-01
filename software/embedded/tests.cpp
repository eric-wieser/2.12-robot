/**
This file contains a set of tests than can be run all by themselves.

To run any one of these tests, add a line to setup() to invoke the function, ie

tests::motor_wiring();

and delete the contents of loop()
*/
#include "tests.h"

#include "Drive.h"
#include "EncoderMeasurement.h"
#include "RobotPosition.h"
#include "MotorController.h"
#include "SerialCommunication.h"
#include "PathPlanner.h"
#include "constants.h"

/**
Tests that the motors are wired up correctly.

When running, verify that the motor and direction printed over serial is what
is observed on the robot.

Also prints the change in encoder values after each movement - expect the
readings to positive, and correspond with the motor that moved
*/
void tests::motor_wiring() {
	Drive md;

	md.resetEncoders();

	{
		Serial.print("Motor R, forward: ");
		uint32_t encr = md.getMREncoder();
		uint32_t encl = md.getMLEncoder();
		Serial.flush();
		md.setMRSpeed(100);
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
		md.setMLSpeed(100);
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

/**
Tests that the encoders work when the motors are spun BY HAND
*/
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

/**
Tests that the encoder measurements are being correctly translated into
wheel speeds
*/
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

/**
Tests that the PID controllers work
*/
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
