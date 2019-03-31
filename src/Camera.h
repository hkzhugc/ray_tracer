#pragma once
#include "math/Vector3D.h"
#include "math/matrix3x3.h"
#include "ray.h"
class Camera
{
public:
	Camera() {};
	Camera(Vector3D _pos, Vector3D _lookat, Vector3D _up, float fovy, size_t _screenW, size_t _screenH);
	Camera(const Camera& cam)
	{
		pos = cam.pos;
		lookat = cam.lookat;
		up = cam.up;
		dir = cam.dir;
		screenW = cam.screenW;
		screenH = cam.screenH;
		c2w = cam.c2w;
	}
	~Camera();

	//input are from 0 to 1
	Ray generate_ray(double x, double y);
	inline const size_t& get_screenW() { return screenW; }
	inline const size_t& get_screenH() { return screenH; }
private :
	double vFov, hFov;
	Vector3D pos;
	Vector3D lookat;
	Vector3D up;
	Vector3D dir;

	size_t screenW, screenH;

	Matrix3x3 c2w;
};

