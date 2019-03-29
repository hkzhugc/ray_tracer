#pragma once
#include "math/Vector3D.h"
#include "ray.h"
#include <float.h>
#include <algorithm>

class BBox
{
public:
	BBox() {
		max = Vector3D(-DBL_MAX, -DBL_MAX, -DBL_MAX);
		min = Vector3D(DBL_MAX, DBL_MAX, DBL_MAX);
		extent = max - min;
	}
	~BBox() {};

	BBox(const Vector3D& p) : min(p), max(p) { extent = max - min; }
	BBox(const Vector3D& _min, const Vector3D& _max) : min(_min), max(_max) { extent = max - min; }
	
	bool intersect(const Ray& r, double& t0, double& t1) const;

	//union two BBox
	void expand(const BBox& _bb)
	{
		min.x = std::min(min.x, _bb.min.x);
		min.y = std::min(min.y, _bb.min.y);
		min.z = std::min(min.z, _bb.min.z);
		max.x = std::max(max.x, _bb.max.x);
		max.y = std::max(max.y, _bb.max.y);
		max.z = std::max(max.z, _bb.max.z);
		extent = max - min;
	}

	void expand(const Vector3D& p) {
		min.x = std::min(min.x, p.x);
		min.y = std::min(min.y, p.y);
		min.z = std::min(min.z, p.z);
		max.x = std::max(max.x, p.x);
		max.y = std::max(max.y, p.y);
		max.z = std::max(max.z, p.z);
		extent = max - min;
	}

	Vector3D centroid() const {
		return (min + max) / 2;
	}

	//rerturn 
	inline double surface_area() 
	{
		return extent.x * extent.y * extent.z;
	}

	Vector3D min;
	Vector3D max;
	Vector3D extent;
private:
	
};

