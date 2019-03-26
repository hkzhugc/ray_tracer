#pragma once
#include "primitive.h"
#include "mesh.h"
class Triangle :
	public Primitive
{
public:
	Triangle(){};
	Triangle(const size_t& _a, const size_t& _b, const size_t& _c) :
		v0(_a), v1(_b), v2(_c), isVertexNormal(false){}
	Triangle(const size_t& _a, const size_t& _b, const size_t& _c, const size_t& _na, const size_t& _nb, const size_t& _nc, BlinnPhonBSDF* _bsdf, Mesh* _mesh) :
		v0(_a), v1(_b), v2(_c), isVertexNormal(true),
		n0(_na), n1(_nb), n2(_nc)
	{
		bsdf = _bsdf;
		mesh = _mesh;
	}
	~Triangle() {};

	BBox get_bbox() const;
	bool intersect(Ray &r) const;
	bool intersect(Ray &r, Intersection *i) const;
	BlinnPhonBSDF* get_bsdf() const { return bsdf; };

	//TODO : it should be private afer debug
	size_t v0, v1, v2;
	size_t n0, n1, n2;
private:
	bool isVertexNormal;
	BlinnPhonBSDF* bsdf;
	Mesh* mesh;
};

