#include "BSDF.h"

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) 
{

	Vector3D z = Vector3D(n.x, n.y, n.z);
	Vector3D h = z;
	if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
	else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
	else h.z = 1.0;

	z.normalize();
	Vector3D y = cross(h, z);
	y.normalize();
	Vector3D x = cross(z, y);
	x.normalize();

	o2w[0] = x;
	o2w[1] = y;
	o2w[2] = z;
}

Color DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf)
{
	*wi = sampler.get_sample(pdf);
	return Kd;
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

Color BlinnPhonBSDF::f(const Vector3D& wo, const Vector3D& wi) const
{
	return Kd / M_PI;
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
