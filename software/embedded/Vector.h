#pragma once

#include <math.h>
#include "Angle.h"

class Vector {
public:
	float x, y;
	Vector(float x, float y) : x(x), y(y) {}
	Vector() : Vector(0, 0) {}

	friend inline Vector operator +(Vector a, Vector b) { return Vector(a.x+b.x, a.y+b.y); }
	friend inline Vector operator -(Vector a, Vector b) { return Vector(a.x-b.x, a.y-b.y); }
	friend inline Vector operator *(Vector a, float f) { return Vector(a.x*f, a.y*f); }
	friend inline Vector operator *(float f, Vector a) { return Vector(a.x*f, a.y*f); }
	friend inline Vector operator /(Vector a, float f) { return Vector(a.x/f, a.y/f); }

	inline float dot(Vector b) {
		return x*b.x + y*b.y;
	}
	inline float magnitudeSq() {
		return dot(*this);
	}
	inline float magnitude() { return sqrt(magnitudeSq()); }

	inline Vector projectionOn(Vector v) {
		return v * (dot(v) / v.magnitudeSq());
	}
	inline Angle angle() {
		return atan2(y, x);
	}
	inline Vector rotCCW() {
		return Vector(-y, x);
	}
	static inline Vector inDirection(Angle a, float l=1) {
		return Vector(cos(a)*l, sin(a)*l);
	}
};
