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

Color BlinnPhonBSDF::sample_f(const Vector3D & wo, Vector3D * wi, float * pdf, bool * is_sample_specular) const
{
	double Xi = random_uniform();
	Color bsdf;

	//TODO : should detect if  pdf_diffuse < 1e-10
	if (Xi < pdf_diffuse + 1e-10 && !(pdf_diffuse < 1e-10))
	{
		bsdf = sample_diffuse(wo, wi, pdf) / pdf_diffuse;
	}
	else if (Xi < pdf_diffuse + pdf_specular + 1e-10 && !(pdf_specular < 1e-10))
	{
		bsdf = sample_specular(wo, wi, pdf) / pdf_specular;
		*is_sample_specular = true;
	}
	else if (!(pdf_transparent < 1e-10))
	{
		//TODO : implement sample transparent
		bsdf = sample_diffuse(wo, wi, pdf) / pdf_diffuse;
		*is_sample_specular = true;
		//bsdf = sample_transparent(wo, wi, pdf) / pdf_transparent;
	}
	if (isnan(bsdf.r) && isnan(bsdf.g) && isnan(bsdf.b))
	{
		printf("isnan sample bsdf %f %f %f , Xi %lf\n", pdf_diffuse, pdf_specular, pdf_transparent, Xi);
	}
	return bsdf;
}

void BlinnPhonBSDF::reflect(const Vector3D& wo, Vector3D* wi) const
{
	*wi = Vector3D(-wo[0], -wo[1], wo[2]);

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
	*wi = 2 * dot(wo, half_vec) * half_vec - wo;
	double reduce_rate = pow(half_vec.z, Ns);

	//void div zero
	if (reduce_rate < 1e-10)
		return Color();

	Color ret = Ks * reduce_rate / fabs((*wi).z);
	//printf("wi %lf %lf %lf, wi norm %lf\n", (*wi).x, (*wi).y, (*wi).z, (*wi).norm());
	//printf("pdf = %f, reduce_rate = %lf\n", *pdf, reduce_rate);
	//printf("ret ks %lf %f %f %f\n", ret.r, ret.g, ret.b);
	//getchar();
	//div the cos_Lin(theta) because we will multi cos_Lin(theta) when estimate the light contribution 
	return ret;
}

Color BlinnPhonBSDF::sample_transparent(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	return Color();
}
