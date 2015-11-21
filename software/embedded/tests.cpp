#include "tests.h"

#include "Drive.h"

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
		Serial.println(diffr);
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
		Serial.println(diffr);
	}
}


void tests::encoder_wiring() {
	Drive md;

	md.resetEncoders();

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