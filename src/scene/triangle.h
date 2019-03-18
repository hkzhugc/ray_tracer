#pragma once
#include "primitive.h"
class Triangle :
	public Primitive
{
public:
	Triangle(){};
	Triangle(const size_t& _a, const size_t& _b, const size_t& _c) :
		v0(_a), v1(_b), v2(_c), isVertexNormal(false){}
	Triangle(const size_t& _a, const size_t& _b, const size_t& _c, const size_t& _na, const size_t& _nb, const size_t& _nc, BSDF* _bsdf) :
		v0(_a), v1(_b), v2(_c), isVertexNormal(true),
		n0(_na), n1(_nb), n2(_nc){
		bsdf = _bsdf;
	}
	~Triangle() {};

	BBox get_bbox() const;
	void intersect(Ray &r) const;
	void intersect(Ray &r, Intersection &i) const;
	BSDF* get_bsdf() const { return bsdf; };
private:
	bool isVertexNormal;
	size_t v0, v1, v2;
	size_t n0, n1, n2;
	BSDF* bsdf;
};

