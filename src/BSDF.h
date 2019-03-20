#pragma once
#include "math/color.h"
#include "math/Vector3D.h"
#include <random>
#include <math.h>

class Sampler
{
public:
	Sampler(){}
	~Sampler(){}

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
class UniformHemisphereSampler3D : public Sampler
{
public:
	UniformHemisphereSampler3D() {};
	~UniformHemisphereSampler3D() {};
	Vector3D get_sample() const;
	Vector3D get_sample(float* pdf) const;
};

class BSDF
{
public:
	BSDF() {};
	~BSDF() {};

	//if Kd length = 0, rerturn true, else return false;
	virtual bool is_delta() const = 0;

	//if bsdf emit lights return the spectrum
	virtual Color get_emission() const = 0;

	//sample a light, return the outgoing dir and pdf and color
	virtual Color sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) = 0;

	//Color Ka;
	//Color Kd;
	//Color Ks;
	//Color Tf;
	//double Ni;
	//double Ns;
};

class BlinnPhonBSDF
{
public:
	BlinnPhonBSDF(Color _Ka, Color _Kd, Color _Ks, Color _Tf, double _Ni, double _Ns) :
		Ka(_Ka), Kd(_Kd), Ks(_Ks), Tf(_Tf), Ni(_Ni), Ns(_Ns)
	{};
	~BlinnPhonBSDF() {};

	void print_bsdf()
	{
		printf("Ka %f %f %f\n", Ka.r, Ka.g, Ka.b);
		printf("Kd %f %f %f\n", Kd.r, Kd.g, Kd.b);
		printf("Ks %f %f %f\n", Ks.r, Ks.g, Ks.b);
		printf("Tf %f %f %f\n", Tf.r, Tf.g, Tf.b);
		printf("Ni %lf\n", Ni);
		printf("Ns %lf\n", Ns);
	}

	Color Ka;
	Color Kd;
	Color Ks;
	Color Tf;
	double Ni;
	double Ns;
};

//Mirror bsdf, when Kd equal (0, 0, 0) and Tf equal (1, 1, 1)
class MirrorBSDF : public BSDF
{
public:
	MirrorBSDF(Color _Ks, double _Ns) : Ks(_Ks), Ns(_Ns) {};
	~MirrorBSDF() {};
	bool is_delta() { return true; };
	Color get_emission() { return Color(); };
	Color sample_f(const Vector3D& wo, Vector3D* wi, float* pdf);
private:
	double Ns;
	Color Ks;
};

//Ks equal (0, 0, 0) and Tf equal (1, 1, 1)
class DiffuseBSDF : public BSDF
{
public:
	DiffuseBSDF(Color _Kd): Kd(_Kd) {};
	~DiffuseBSDF() {};
	bool is_delta() { return false; };
	Color get_emission() { return Color(); };
	Color sample_f(const Vector3D& wo, Vector3D* wi, float* pdf);
private:
	Color Kd;
	CosineWeightedHemisphereSampler sampler;
};