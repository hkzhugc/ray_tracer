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
	: public Light
{
public:
	SphereLight(const Color _radiance, const Vector3D& _pos, const double& _radius) : radiance(_radiance), pos(_pos), radius(_radius) {}
	~SphereLight() = default;
	Color sample_L(const Vector3D& p, Vector3D* wi, float* distToLight, float* pdf) const;
private:
	Vector3D pos;
	double radius;
	UniformSphereSampler3D sampler;
	Color radiance;
};

