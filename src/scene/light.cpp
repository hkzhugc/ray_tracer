#include "light.h"

Color SphereLight::sample_L(const Vector3D & p, Vector3D * wi, float * distToLight, float * pdf)
{
	Vector3D sample_dir = sampler.get_sample(pdf);
	Vector3D light_pos = pos + sample_dir * radius;
	Vector3D light_dir = light_pos - p;
	double dist2 = light_dir.norm2();
	*distToLight = light_dir.norm();
	*wi = light_dir / (*distToLight);
	double cos_theta = dot(sample_dir, light_dir);
	if (cos_theta <= 0)
		return Color();
	else
		return radiance * cos_theta / dist2;
}
