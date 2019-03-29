#pragma once
#include "math/color.h"
#include "math/Vector3D.h"
#include "math/matrix3x3.h"
#include "sampler.h"
#include <random>
#include <math.h>
#include <string>
using std::string;

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
	BlinnPhonBSDF(string _name, Color _Ka, Color _Kd, Color _Ks, Color _Tf, double _Ni, double _Ns) :
		name(_name), Ka(_Ka), Kd(_Kd), Ks(_Ks), Tf(_Tf), Ni(_Ni), Ns(_Ns), specular_sampler(_Ns)
	{
		pdf_diffuse = Kd.illum();
		pdf_specular = Ks.illum();
		pdf_transparent = Color(1 - Tf.r, 1 - Tf.g, 1 - Tf.b).illum();

		double sum = pdf_diffuse + pdf_specular + pdf_transparent;
		pdf_diffuse /= sum;
		pdf_specular /= sum;
		pdf_transparent /= sum;
	};
	~BlinnPhonBSDF() {};

	void print_bsdf()
	{
		printf("bsdf name is %s\n", name.c_str());
		printf("Ka %f %f %f\n", Ka.r, Ka.g, Ka.b);
		printf("Kd %f %f %f\n", Kd.r, Kd.g, Kd.b);
		printf("Ks %f %f %f\n", Ks.r, Ks.g, Ks.b);
		printf("Tf %f %f %f\n", Tf.r, Tf.g, Tf.b);
		printf("Ni %lf\n", Ni);
		printf("Ns %lf\n", Ns);
	}

	//if bsdf emit lights return the spectrum
	Color get_emission() const;

	//sample a light, return the outgoing dir and pdf and color
	Color sample_f(const Vector3D& wo, Vector3D* wi, float* pdf, bool * is_sample_specular) const;
	void reflect(const Vector3D & wo, Vector3D * wi) const;
	Color f(const Vector3D& wo, const Vector3D& wi) const;
	Color Kd;
private:
	string name;

	Color Ka;
	
	Color Ks;
	Color Tf;
	double Ni;
	double Ns;

	//pdf for sample which the light came from
	double pdf_diffuse;
	double pdf_specular;
	double pdf_transparent;

	CosineWeightedHemisphereSampler diffuse_sampler;
	CosineNPowWeightedHemisphereSampler specular_sampler;

	Color sample_diffuse(const Vector3D& wo, Vector3D* wi, float* pdf) const;
	Color sample_specular(const Vector3D& wo, Vector3D* wi, float* pdf) const;
	Color sample_transparent(const Vector3D& wo, Vector3D* wi, float* pdf) const;
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

void make_coord_space(Matrix3x3& o2w, const Vector3D& n);