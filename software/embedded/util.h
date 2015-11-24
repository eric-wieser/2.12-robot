#pragma once

template<typename T>
T lerp(T a, T b, float p) {
	return a*(1-p) + b*p;
}

template<typename T>
float unlerp(T a, T b, T lerped) {
	return (lerped - a) / (b - a);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}