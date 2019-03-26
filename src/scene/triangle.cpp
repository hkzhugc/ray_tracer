#include "triangle.h"

BBox Triangle::get_bbox() const
{
	return BBox();
}


bool Triangle::intersect(Ray &r) const
{
	Intersection temp;
	return intersect(r, &temp);
}

bool Triangle::intersect(Ray &r, Intersection *isect) const
{
	Vector3D p1(mesh->vertices[v0]), p2(mesh->vertices[v1]), p3(mesh->vertices[v2]);
	Vector3D n1(mesh->normals[this->n0]), n2(mesh->normals[this->n1]), n3(mesh->normals[this->n2]);
	bool flag = false;
	double b1, b2, t;
	Vector3D S, S1, S2, E1, E2;
	E1 = p2 - p1;
	E2 = p3 - p1;
	S = r.o - p1;
	S1 = cross(r.d, E2);
	S2 = cross(S, E1);
	double invS1DotE = (1.0 / dot(S1, E1));
	b1 = invS1DotE * dot(S1, S);
	if (b1 < 0 || b1 > 1)
		return flag;
	b2 = invS1DotE * dot(S2, r.d);
	if (b2 < 0 || b2 > 1)
		return flag;
	if (b1 + b2 > 1)
		return flag;
	t = invS1DotE * dot(S2, E2);
	if (t < r.min_t || t > r.max_t)
		return flag;
	//update max_t 
	r.max_t = t;
	flag = true;
	if (isect != NULL)
	{
		isect->t = t;
		isect->n = (1 - b1 - b2) * n1 + b1 * n2 + b2 * n3;
		isect->primitive = this;
		isect->bsdf = get_bsdf();
	}
	return flag;
}