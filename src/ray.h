#pragma once
#include "math/Vector3D.h"
#include <float.h>
struct Ray
{
	~Ray(){};
	
	Ray(const Vector3D &_o, const Vector3D &_d) : o(_o), d(_d), min_t(0.0), max_t(DBL_MAX), depth(3) {}
	Ray(const Vector3D &_o, const Vector3D &_d, const double &_max_t) : o(_o), d(_d), min_t(0.0), max_t(_max_t), depth(3){}

	Vector3D o;//ray start at o
	Vector3D d;//ray trans along d 
	double min_t, max_t;//ray exsitence
	size_t depth;

	//return the ray at time t
	inline Vector3D at_time(double t) const { return o + t * d; }
};