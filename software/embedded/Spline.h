#pragma once

//https://en.wikipedia.org/wiki/Cubic_Hermite_spline
class Spline {
public:
	const float a0, a1, a2, a3;

	// turn values and derivatives into coefficients
	Spline(float x0, float x1, float dx0, float dx1) :
		a0(x0),
		a1(dx0),
		a2(-3*x0 + 3*x1 - 2*dx0 - dx1),
		a3( 2*x0 - 2*x1 +   dx0 + dx1) { }

	// evaluate at p, for 0 <= p <= 1
	float eval(float p) const {
		if(p < 0) p = 0;
		if(p > 1) p = 1;
		float p2 = p*p;
		float p3 = p2*p;
		return a3*p3 + a2*p2 + a1*p + a0;
	}

	// evaluate the gradient at p, for 0 <= p <= 1
	float eval_dp(float p) const {
		if(p < 0) p = 0;
		if(p > 1) p = 1;
		return 3*a3*p*p + 2*a2*p + a1;
	}
};
