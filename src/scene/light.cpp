#include "light.h"

Color SphereLight::sample_L(const Vector3D & p, Vector3D * wi, double * distToLight, float * pdf) const
{
	Vector3D sample_dir = sampler.get_sample(pdf);
	*pdf /= (radius * radius);
	Vector3D light_pos = pos + sample_dir * (radius + 1e-3);
	Vector3D light_dir = light_pos - p;
	Vector3D dir = light_dir.unit();
	double dist2 = light_dir.norm2();
	*distToLight = light_dir.norm();
	*wi = dir;
	double cos_theta = -dot(sample_dir, dir);
	if (cos_theta <= 0)
		return Color(0, 0, 0);
	else
		return radiance * cos_theta / dist2;
}


Color RecangleLight::sample_L(const Vector3D & p, Vector3D * wi, double * distToLight, float * pdf) const
{
	double X1, X2;
	X1 = random_uniform() - 0.5; // -0.5 to 0.5
	X2 = random_uniform() - 0.5;// -0.5 to 0.5

	double offsetx = X1 * size_x;
	double offsety = X1 * size_y;

	//since in this case, light normal are (1, 0, 0)
	Vector3D light_pos = center + Vector3D(0, offsetx, offsety);
	Vector3D light_dir = light_pos - p;
	Vector3D dir = light_dir.unit();

	double dist2 = light_dir.norm2();
	*distToLight = light_dir.norm();
	*wi = dir;

	double cos_theta = -dot(dir, normal);
	*pdf = 1 / (area * fabs(cos_theta)); // radiance reduce term to pdf, improve the accurancy

	if (cos_theta > 0)
		return radiance  * fabs(cos_theta) / dist2;
	else 
		return Color(0, 0, 0);
}
