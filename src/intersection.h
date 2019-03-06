#pragma once
#include "math/Vector3D.h"
#include "BSDF.h"
class Primitive;

class Intersection
{
public:
	Intersection();
	~Intersection();

	double t; //intersect的时间
	Vector3D n; //交点的normal
	Primitive* p; //相交的多边形
	BSDF* bsdf; //交点的bsdf
};

Intersection::Intersection()
{
}

Intersection::~Intersection()
{
}