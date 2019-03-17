#pragma once

#include "../BBox.h"
#include "../ray.h"
#include "../intersection.h"
#include "../BSDF.h"

//define a interface to represent the Primitive(triangles etc.)
class Primitive
{
public:
	//return AABB for building bvh
	virtual BBox get_bbox() const = 0;

	virtual void intersect(Ray &r) const = 0;

	virtual void intersect(Ray &r, Intersection &i) const = 0;

	virtual BSDF* get_bsdf() const = 0;
	 
private:

};