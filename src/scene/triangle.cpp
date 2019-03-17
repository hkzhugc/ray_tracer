#include "triangle.h"

Vector3D Triangle::get_normal(double b1, double b2)
{
	if (!isVertexNormal)
		return n0;
	else
		return n0 * (1 - b1 - b2) + n1 * b1 + n2 * b2;
}

BBox Triangle::get_bbox() const
{
	return BBox();
}


void Triangle::intersect(Ray &r) const
{

}

void Triangle::intersect(Ray &r, Intersection &i) const
{

}