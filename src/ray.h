#pragma once
#include "math/Vector3D.h"
#include <float.h>
struct Ray
{
	~Ray();
	Vector3D o;//ray的起点
	Vector3D d;//ray的方向
	double min_t, max_t;//ray的有效期

	Ray(const Vector3D &_o, const Vector3D &_d) : o(_o), d(_d), min_t(0.0), max_t(DBL_MAX) {}
	Ray(const Vector3D &_o, const Vector3D &_d, const double &_max_t) : o(_o), d(_d), min_t(0.0), max_t(_max_t) {}

	//返回t时刻光线所在的位置
	inline Vector3D at_time(double t) const { return o + t * d; }
};