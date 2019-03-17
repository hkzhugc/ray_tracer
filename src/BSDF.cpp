#include "BSDF.h"

double random_uniform()
{
	return ((double)std::rand()) / RAND_MAX;
}

Vector3D CosineWeightedHemisphereSampler::get_sample() const
{
	float f;
	return get_sample(&f);
}

Vector3D CosineWeightedHemisphereSampler::get_sample(float *pdf) const
{
	double Xi1 = random_uniform(); // sample sin(theta) square? because we should sample the point according to sin(theta)cos(theta), which PDF = sin(theta) square
	double Xi2 = random_uniform();

	double r = sqrt(Xi1); // r equal to sin theta
	double phi = 2. * M_PI * Xi2;
	*pdf = sqrt(1 - Xi1) / M_PI;// pdf = 2 cos(theta) / 2Pi
	return Vector3D(r*cos(phi), r*sin(phi), sqrt(1 - Xi1));
}

Vector3D UniformHemisphereSampler3D::get_sample() const
{
	float f;
	return get_sample(&f);
}

Vector3D UniformHemisphereSampler3D::get_sample(float *pdf) const
{
	*pdf = 1 / 2. * M_PI;
	double Xi1 = random_uniform();
	double Xi2 = random_uniform();

	double theta = acos(Xi1);
	double phi = 2.0 * M_PI * Xi2;

	double xs = sinf(theta) * cosf(phi);
	double ys = sinf(theta) * sinf(phi);
	double zs = cosf(theta);

	return Vector3D(xs, ys, zs);
}

Color DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf)
{
	*wi = sampler.get_sample(pdf);
	return Kd;
}