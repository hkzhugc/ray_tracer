#include "Camera.h"


Camera::Camera()
{
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
	dir = c2w * dir;

	return Ray(loc, dir.unit());
}