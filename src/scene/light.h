#pragma once
#include "../sampler.h"
#include "../math/color.h"
class Light
{
public:
	Light() {}
	virtual ~Light() {}
	virtual Color sample_L(const Vector3D& p, Vector3D* wi, double* distToLight, float* pdf) const = 0;
};

class SphereLight
	: public Light
{
public:
	SphereLight(const Color _radiance, const Vector3D& _pos, const double& _radius) : radiance(_radiance), pos(_pos), radius(_radius) {}
	~SphereLight() = default;
	Color sample_L(const Vector3D& p, Vector3D* wi, double* distToLight, float* pdf) const;
private:
	Vector3D pos;
	double radius;
	UniformSphereSampler3D sampler;
	Color radiance;
};

class TriangleLight
	: public Light
{
public:
	TriangleLight(const Color _radiance, const Vector3D& _p1, const Vector3D& _p2, const Vector3D& _p3, const Vector3D& _n1, const Vector3D& _n2, const Vector3D& _n3)
		: radiance(_radiance), p1(_p1), p2(_p2), p3(_p3),
		n1(_n1), n2(_n2), n3(_n3)
	{}
	~TriangleLight() = default;
	Color sample_L(const Vector3D& p, Vector3D* wi, double* distToLight, float* pdf) const;
private:
	Vector3D p1, p2, p3;
	Vector3D n1, n2, n3;
	UniformSphereSampler3D sampler;
	Color radiance;
};

