#pragma once
#include "math/Vector3D.h"
#include <algorithm>
using std::max;
class Sampler
{
public:
	Sampler() {}
	~Sampler() {}

	virtual Vector3D get_sample() const = 0;
	virtual Vector3D get_sample(float* pdf) const = 0;
};

class CosineWeightedHemisphereSampler : public Sampler
{
public:
	CosineWeightedHemisphereSampler() {};
	~CosineWeightedHemisphereSampler() {};
	Vector3D get_sample() const;
	Vector3D get_sample(float* pdf) const;
};

class CosineNPowWeightedHemisphereSampler : public Sampler
{
public:
	CosineNPowWeightedHemisphereSampler(double _Ks) : Ks(_Ks) {};
	~CosineNPowWeightedHemisphereSampler() {};
	Vector3D get_sample() const;
	Vector3D get_sample(float* pdf) const;
private:
	double Ks;
};

class UniformHemisphereSampler3D : public Sampler
{
public:
	UniformHemisphereSampler3D() {};
	~UniformHemisphereSampler3D() {};
	Vector3D get_sample() const;
	Vector3D get_sample(float* pdf) const;
};

class UniformSphereSampler3D : public Sampler
{
public:
	UniformSphereSampler3D() {};
	~UniformSphereSampler3D() {};
	Vector3D get_sample() const;
	Vector3D get_sample(float* pdf) const;
};

double random_uniform();