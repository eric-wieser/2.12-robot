#pragma once

/**
A class representing an angle, and dealing with wraparound in [-PI, PI)

	Angle + float -> Angle
	Angle - float -> Angle
*/
class Angle {
private:
	mutable float _value;
	static float normalize(float theta);
public:
	Angle() {}
	Angle(float f) : _value(f) {};

	// type preserving operators
	friend inline Angle operator +(Angle a, float da) { return a._value + da; }
	friend inline Angle operator +(float da, Angle a) { return a._value + da; }
	friend inline Angle operator -(Angle a, float da) { return a._value - da; }

	// conversion operators
	friend inline float operator -(Angle a, Angle b) { return normalize(a._value - b._value); }
	inline operator float() const { return _value = normalize(_value); }

	// augmented assignment operators
	inline Angle& operator +=(float da) { _value += da; return *this; }
	inline Angle& operator -=(float da) { _value -= da; return *this; }

	// places where conversion to float isn't necessary
	friend inline float cos(Angle a) { return cos(a._value); }
	friend inline float sin(Angle a) { return sin(a._value); }
	friend inline float tan(Angle a) { return tan(a._value); }
};
