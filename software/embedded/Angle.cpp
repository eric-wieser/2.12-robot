#include "Angle.h"

#include <math.h>

float Angle::normalize(float theta) {
	while(theta >= M_PI) theta -= 2*M_PI;
	while(theta < -M_PI) theta += 2*M_PI;
	return theta;
}