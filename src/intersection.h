#pragma once
#include "math/Vector3D.h"
#include "BSDF.h"
class Primitive;

class Intersection
{
public:
	Intersection(){};
	~Intersection(){};

	double t; //intersect t
	Vector3D n; //normal
	const Primitive* primitive; //intersect with p
	BlinnPhonBSDF* bsdf; //bsdf
};