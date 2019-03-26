#pragma once
#include "math/Vector3D.h"
#include "math/matrix3x3.h"
#include "ray.h"
class Camera
{
public:
	Camera(Vector3D _pos, Vector3D _lookat, Vector3D _up, float fovy, size_t _screenW, size_t _screenH);
	~Camera();

	//input are from 0 to 1
	Ray generate_ray(double x, double y);
private :
	double vFov, hFov;
	Vector3D pos;
	Vector3D lookat;
	Vector3D up;
	Vector3D dir;

	size_t screenW, screenH;

	Matrix3x3 c2w;
};

