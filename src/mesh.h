#pragma once
#include <vector>
#include <map>
#include <string>
#include "math/Vector3D.h"
#include "BSDF.h"
using std::vector;
using std::map;
using std::string;
typedef map<string, BSDF*> Materials;
class Mesh
{
public:
	Mesh(){};
	~Mesh(){};
	void Load_Scene(string material_file, string mesh_file, string light_file);
	vector<Vector3D> vertices;
	vector<Vector3D> vertex_normals;
	Materials materials;
};

