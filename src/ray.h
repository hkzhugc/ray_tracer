#pragma once
#include "math/Vector3D.h"
#include <float.h>
struct Ray
{
	~Ray();
	Vector3D o;//ray�����
	Vector3D d;//ray�ķ���
	double min_t, max_t;//ray����Ч��

	Ray(const Vector3D &_o, const Vector3D &_d) : o(_o), d(_d), min_t(0.0), max_t(DBL_MAX) {}
	Ray(const Vector3D &_o, const Vector3D &_d, const double &_max_t) : o(_o), d(_d), min_t(0.0), max_t(_max_t) {}

	//����tʱ�̹������ڵ�λ��
	inline Vector3D at_time(double t) const { return o + t * d; }
};