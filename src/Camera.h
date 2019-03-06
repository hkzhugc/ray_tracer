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

	size_t screenW, screenH;//分辨率

	Matrix3x3 c2w;

	//根据屏幕x,y位置，生成光线 (取值0到1)
	Ray generate_ray(double x, double y);
};

