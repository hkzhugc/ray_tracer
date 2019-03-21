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
	*pdf = 1 / (2. * M_PI);
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

Vector3D CosineNPowWeightedHemisphereSampler::get_sample() const
{
	float pdf;
	return get_sample(&pdf);
}

Vector3D CosineNPowWeightedHemisphereSampler::get_sample(float * pdf) const
{
	double Xi1 = random_uniform();
	double Xi2 = random_uniform();

	double cos_val = pow(Xi1, 1. / (Ks + 1));
	double phi = 2.0 * M_PI * Xi2;
	double sin_val = sqrt(1 - cos_val * cos_val);

	*pdf = (Ks + 1) * pow(cos_val, Ks) / (2. * M_PI);

	double xs = sin_val * cosf(phi);
	double ys = sin_val * sinf(phi);
	double zs = cos_val;

	return Vector3D(xs, ys, zs);
}

Color BlinnPhonBSDF::get_emission() const
{
	return Ka;
}

Color BlinnPhonBSDF::sample_f(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	double Xi = random_uniform();
	Color bsdf;
	if (Xi < pdf_diffuse)
	{
		bsdf = sample_diffuse(wo, wi, pdf) / pdf_diffuse;
	}
	else if (Xi < pdf_diffuse + pdf_specular)
	{
		bsdf = sample_specular(wo, wi, pdf) / pdf_specular;
	}
	else
	{
		bsdf = sample_transparent(wo, wi, pdf) / pdf_transparent;
	}
	return bsdf;
}

Color BlinnPhonBSDF::sample_diffuse(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	*wi = diffuse_sampler.get_sample(pdf);
	return Kd / M_PI;
}

Color BlinnPhonBSDF::sample_specular(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	//wi was the half vector, must calculate the income vector
	Vector3D half_vec = specular_sampler.get_sample(pdf);
	*wi = dot(wo, half_vec) * half_vec - wo;
	double reduce_rate = pow(half_vec.z, Ns);

	//div the cos_Lin(theta) because we will multi cos_Lin(theta) when estimate the light contribution 
	return Ks * reduce_rate / fabs((*wi).z);
}

Color BlinnPhonBSDF::sample_transparent(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	return Color();
}
