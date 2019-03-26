#pragma once
#include "../sampler.h"
#include "../math/color.h"
class Light
{
public:
	Light() {}
	virtual ~Light() {}
	virtual Color sample_L(const Vector3D& p, Vector3D* wi, float* distToLight, float* pdf) const = 0;
};

class SphereLight
	: Light
{
public:
	SphereLight(Color _radiance) : radiance(_radiance) {}
	~SphereLight() = default;
	Color sample_L(const Vector3D& p, Vector3D* wi, float* distToLight, float* pdf);
private:
	Vector3D pos;
	double radius;
	UniformSphereSampler3D sampler;
	Color radiance;
};

