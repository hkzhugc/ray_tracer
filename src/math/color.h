#pragma once
#include <math.h>
class Color {
public:
	float r;
	float g;
	float b;

	Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

	inline Color operator+(const Color &rhs) const {
		return Color(r + rhs.r, g + rhs.g, b + rhs.b);
	}

	inline Color &operator+=(const Color &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		return *this;
	}

	inline Color operator*(const Color &rhs) const {
		return Color(r * rhs.r, g * rhs.g, b * rhs.b);
	}

	inline Color &operator*=(const Color &rhs) {
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		return *this;
	}

	inline Color operator*(float s) const {
		return Color(r * s, g * s, b * s);
	}

	inline Color &operator*=(float s) {
		r *= s;
		g *= s;
		b *= s;
		return *this;
	}

	inline Color operator/(float s) const {
		return Color(r / s, g / s, b / s);
	}

	inline Color &operator/=(float s) {
		r /= s;
		g /= s;
		b /= s;
		return *this;
	}

	inline bool operator==(const Color &rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b;
	}

	inline bool operator!=(const Color &rhs) const {
		return !operator==(rhs);
	}

	inline float illum() const {
		return 0.2126f * r + 0.7152f * g + 0.0722f * b;
	}

	inline void gama()
	{
		r = sqrt(r);
		g = sqrt(g);
		b = sqrt(b);
	}
};