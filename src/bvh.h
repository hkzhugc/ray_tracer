#pragma once
#include "BBox.h"
#include "scene/triangle.h"
#include "intersection.h"

struct BVHNode
{
	BVHNode(BBox _bb) : l(NULL), r(NULL), bb(_bb), triangles(NULL) {};
	~BVHNode() { if (triangles) delete triangles; if (l) delete l; if (r) delete r; }
	BVHNode* l;
	BVHNode* r;
	vector<Triangle*> *triangles;
	BBox bb;

	inline bool isLeaf() const { return l == NULL && r == NULL; }
};

class BVH
{
public:
	BVH() { root = NULL; }
	~BVH() { if (root) delete root; };

	void build_bvh(const vector<Triangle*> &triangles);

	//when meet intersect return imediately
	bool intersect(Ray& r) const {
		return intersect(r, root);
	}

	//meet all possible intersect
	bool intersect(Ray& r, Intersection* i) const {
		return intersect(r, i, root);
	}
private:
	BVHNode *construct_bvh(const std::vector<Triangle*>& prims);
	bool intersect(Ray& r, BVHNode* node) const;
	bool intersect(Ray& r, Intersection* i, BVHNode* node) const;
	BVHNode* root;
	
};


struct BBoxBucket {
	BBox bbox;
	int prim_count;
	std::vector<Triangle *> p_list;
	BBoxBucket() : bbox(), p_list() { prim_count = 0; p_list.clear(); }
	void expand(BBoxBucket &bbb)
	{
		bbox.expand(bbb.bbox);
		p_list.insert(p_list.end(), bbb.p_list.begin(), bbb.p_list.end());
		prim_count += bbb.prim_count;
	}
	void expand(Triangle *p)
	{
		bbox.expand(p->get_bbox());
		p_list.push_back(p);
		prim_count++;
	}
	double get_cost()
	{
		return bbox.surface_area() * (double)p_list.size();
	}
};
