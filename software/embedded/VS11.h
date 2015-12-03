#pragma once

#include <Servo.h>

class VS11 : private Servo {
public:
	VS11(int pin=5);

	void setGoal(float goal);
};
