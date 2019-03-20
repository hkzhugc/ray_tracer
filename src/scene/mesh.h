#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "light.h"
#include "triangle.h"
#include "../math/Vector3D.h"
#include "../BSDF.h"
using std::vector;
using std::map;
using std::string;
using std::ifstream;
using std::istringstream;

class Mesh;
class Token_processer;
typedef map<string, BlinnPhonBSDF*> Materials;
typedef map<string, Token_processer*> Token_processers;


class Token_processer
{
public:
	virtual void process(vector<string>& words, Mesh& mesh) const = 0;
};

class Token_processer_vertex_or_normal : public Token_processer
{
public:
	Token_processer_vertex_or_normal(){};
	~Token_processer_vertex_or_normal() {};
	void process(vector<string>& words, Mesh& mesh) const;
};

class Token_processer_mtllib : public Token_processer
{
public:
	Token_processer_mtllib(){};
	~Token_processer_mtllib() {};
	void process(vector<string>& words, Mesh& mesh) const;
};

class Token_processer_usemtl : public Token_processer
{
public:
	Token_processer_usemtl(){};
	~Token_processer_usemtl() {};
	void process(vector<string>& words, Mesh& mesh) const;
};

class Token_processer_face : public Token_processer
{
public:
	Token_processer_face(){};
	~Token_processer_face() {};
	void process(vector<string>& words, Mesh& mesh) const;
};
class Mesh
{
public:
	Mesh()
	{
		Token_processer_vertex_or_normal* tpvn = new Token_processer_vertex_or_normal;
		Token_processer_mtllib* tpmtl = new Token_processer_mtllib;
		Token_processer_face* tpf = new Token_processer_face;
		token_processers["v"] = tpvn;
		token_processers["vn"] = tpvn;
		token_processers["mtllib"] = tpmtl;
		token_processers["f"] = tpf;
	}
	~Mesh()
	{
		for (auto e : materials)
		{
			delete e.second;
		}
	}
	void Load_Scene(string mesh_file, string light_file);
	void Load_Light(string light_file);
	void Load_Material(string material_file);
	void Print_Mesh();
	vector<Vector3D> vertices;
	vector<Vector3D> vertex_normals;
	vector<Light> lights;
	vector<Triangle> primitives;
	Materials materials;
	Token_processers token_processers;
private:
	void Process_token(vector<string>& words);
	string prefix;
};

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

