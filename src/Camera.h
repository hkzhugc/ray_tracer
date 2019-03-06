#pragma once
#include "math/Vector3D.h"
#include "math/matrix3x3.h"
#include "ray.h"
class Camera
{
public:
	Camera();
	~Camera();

	double vFov, hFov;
	Vector3D loc;
	Vector3D up;
	Vector3D dir;

	size_t screenW, screenH;//�ֱ���

	Matrix3x3 c2w;

	//������Ļx,yλ�ã����ɹ��� (ȡֵ0��1)
	Ray generate_ray(double x, double y);
};

