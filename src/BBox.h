#pragma once
#include "math/Vector3D.h"
class BBox
{
public:
	BBox();
	~BBox();
	BBox(const Vector3D& _min, const Vector3D& _max) : min(_min), max(_max) {}
private:
	Vector3D min;
	Vector3D max;
};

