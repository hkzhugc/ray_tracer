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
	if (pdf_diffuse > 1e-10 && Xi < pdf_diffuse + 1e-10)
	{
		bsdf = sample_diffuse(wo, wi, pdf) / pdf_diffuse;
	}
	else if (pdf_specular > 1e-10 && Xi < pdf_diffuse + pdf_specular + 1e-10)
	{
		bsdf = sample_specular(wo, wi, pdf) / pdf_specular;
		*is_sample_specular = true;
	}
	else if (pdf_transparent > 1e-10)
	{
		bsdf = sample_transparent(wo, wi, pdf) / pdf_transparent;
		*is_sample_specular = true;
	}
	else
	{
		//printf("nothing %lf %lf %lf %lf\n", Xi, pdf_diffuse, pdf_specular, pdf_transparent);
		return Color();
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

bool BlinnPhonBSDF::refract(const Vector3D& wo, Vector3D* wi, float Ni) const
{
	int sign = (wo.z >= 0 ? -1 : 1);
	float eta = (wo.z >= 0 ? (1. / Ni) : Ni);
	float intervalDelta = 1. - eta * eta * (1. - wo.z * wo.z);
	if (intervalDelta < 0) return false;
	*wi = Vector3D(-eta * wo[0], -eta * wo[1], (float)sign * sqrt(intervalDelta));
	if (wi->z * wo.z > 0) printf("wrong\n");
	return true;
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
	return ret;
}

Color BlinnPhonBSDF::sample_transparent(const Vector3D & wo, Vector3D * wi, float * pdf) const
{
	*pdf = 1;
	if (refract(wo, wi, Ni))
	{
		//printf("refract %lf\n", Ni);
		return Color(1, 1, 1) / fabs((*wi).z);
	}
	else
	{
		//printf("reflect %lf\n", Ni);
		reflect(wo, wi);
		return Color(1, 1, 1) / fabs((*wi).z);
	}
	//if (reflect_flag)
	//{
	//	reflect(wo, wi);
	//	return Color(*pdf, *pdf, *pdf) / fabs((*wi).z);
	//}
	//else
	//{
	//	refract(wo, wi, Ni);
	//	if (wo.z >= 0) eta = 1.0 / Ni;
	//	else eta = Ni;
	//	if (wi->z * wo.z > 0) printf("wrong\n");
	//	return Color(1 - Tf.r, 1- Tf.g, 1 - Tf.b) * (*pdf) / fabs((*wi).z) / pow(eta, 2);
	//}
}
