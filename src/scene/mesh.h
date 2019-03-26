#pragma once
#include <vector>
#include "../math/Vector3D.h"
using std::vector;

class Mesh
{
public:
	Mesh() {};
	~Mesh() {};

	vector<Vector3D> vertices;
	vector<Vector3D> normals;
};
