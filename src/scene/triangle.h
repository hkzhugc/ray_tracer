#pragma once
#include "primitive.h"
class Triangle :
	public Primitive
{
public:
	Triangle(){};
	Triangle(const Vector3D& _a, const Vector3D& _b, const Vector3D& _c) : 
		v0(_a), v1(_b), v2(_c), isVertexNormal(false)
	{
		Vector3D normal;
		normal = cross(v1 - v0, v2 - v1);
		normal.normalize();
		n0 = n1 = n2 = normal;
	}
	Triangle(const Vector3D& _a, const Vector3D& _b, const Vector3D& _c, const Vector3D& _na, const Vector3D& _nb, const Vector3D& _nc) : 
		v0(_a), v1(_b), v2(_c), isVertexNormal(true),
		n0(_na), n1(_nb), n2(_nc){}
	~Triangle() {};

	BBox get_bbox() const;
	void intersect(Ray &r) const;
	void intersect(Ray &r, Intersection &i) const;
	BSDF* get_bsdf() const;
	Vector3D get_normal(double b1, double b2);
private:
	bool isVertexNormal;
	Vector3D v0, v1, v2;
	Vector3D n0, n1, n2;
};

