#include "VS11.h"
#include "Arduino.h"
#include "Servo.h"

VS11::VS11(float orientation) {
	#define SERVO_arm 5 
	Servo link3;

	 void setup(){
	   pinMode(SERVO_arm, OUTPUT);
	   link3.attach(SERVO);
	 }
	 void loop(){
	   link3.write(orientation);
	   delay(100);
	 }
}

void VS11::setGoal(float goal) {
	/* adjust PWM frequency here
	From the documentation (http://www.vigorprecision.com.hk/uploadfile/20120530/20120530150204367.pdf):
		 700us - one extreme
		1500us - center position
		2200us - other extreme

	There's no mention of PWM period, but 3000us seems likely
	*/
}