#pragma once
#include "math/Vector3D.h"
#include <float.h>
struct Ray
{
	~Ray(){};
	
	Ray(const Vector3D &_o, const Vector3D &_d) : o(_o), d(_d), min_t(0.0), max_t(DBL_MAX), depth(10) 
	{
		inv_d = Vector3D(1 / d.x, 1 / d.y, 1 / d.z);
		sign[0] = (inv_d.x < 0);
		sign[1] = (inv_d.y < 0);
		sign[2] = (inv_d.z < 0);
	}
	Ray(const Vector3D &_o, const Vector3D &_d, const double &_max_t) : o(_o), d(_d), min_t(0.0), max_t(_max_t), depth(3)
	{
		inv_d = Vector3D(1 / d.x, 1 / d.y, 1 / d.z);
		sign[0] = (inv_d.x < 0);
		sign[1] = (inv_d.y < 0);
		sign[2] = (inv_d.z < 0);
	}

	Vector3D o;//ray start at o
	Vector3D d;//ray trans along d 
	double min_t, max_t;//ray exsitence
	size_t depth;

	Vector3D inv_d; //inverse dir, for cal bbox intersect
	int sign[3]; //direction sign

	//return the ray at time t
	inline Vector3D at_time(double t) const { return o + t * d; }
};