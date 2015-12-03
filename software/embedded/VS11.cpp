#include "VS11.h"
#include "Arduino.h"

VS11::VS11(int pin) {
	// configure timers and PWM here
	Servo::attach(pin, 700, 2200);
}

void VS11::setGoal(float goal) {
	/* adjust PWM frequency here
	From the documentation (http://www.vigorprecision.com.hk/uploadfile/20120530/20120530150204367.pdf):
		 700us - one extreme
		1500us - center position
		2200us - other extreme

	There's no mention of PWM period, but 3000us seems likely
	*/
	Servo::write(180 / PI * goal);
}
