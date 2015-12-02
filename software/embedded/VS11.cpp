#include "VS11.h"

VS11::VS11() {
	// configure timers and PWM here
}

void VS11::setGoal(float goal) {
	/* adjust PWM frequency here
	From the documentationL:
		 700us - one extreme
		1500us - center position
		2200us - other extreme

	There's no mention of PWM period, but 3000us seems likely
	*/
}