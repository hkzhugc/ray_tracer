#include "Camera.h"

Camera::Camera(Vector3D _pos, Vector3D _lookat, Vector3D _up, float fovy, size_t _screenW, size_t _screenH)
	: pos(_pos), lookat(_lookat), screenW(_screenW), screenH(_screenH)
{
	Vector3D dir2Camera = (pos - lookat).unit();
	Vector3D cameraXDir = cross(_up, dir2Camera);
	cameraXDir.normalize();
	up = cross(dir2Camera, cameraXDir);
	up.normalize();

	c2w[0] = cameraXDir;
	c2w[1] = up;
	c2w[2] = dir2Camera;

	vFov = fovy;
	hFov = fovy * screenW / screenH;
}

Camera::~Camera()
{
}

double radians(double degree)
{
	return degree / 180.0 * M_PI;
}

Ray Camera::generate_ray(double x, double y)
{
	Vector3D dir = Vector3D((2.0 * x - 1.0) * tan(radians(hFov)*0.5), (2.0 * y - 1.0) * tan(radians(vFov)*0.5), -1);
	dir.normalize();
	dir = c2w * dir;

	return Ray(pos, dir.unit());
}