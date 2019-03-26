#include "scene.h"

template <class Type>
void stringToNum(Type& num,const string& str)
{
	istringstream iss(str);
	iss >> num;
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void Token_processer_vertex_or_normal::process(vector<string>& words, Scene& scene) const
{
	if (words.size() != 4)
	{
		printf("wrong line in file\n");
		return;
	}
	bool isNormal = (words[0] == "vn");
	vector<Vector3D>& tar_vec = (isNormal) ? scene.mesh.normals : scene.mesh.vertices;
	Vector3D val;
	stringToNum(val.x, words[1]);
	stringToNum(val.y, words[2]);
	stringToNum(val.z, words[3]);
	tar_vec.push_back(val);
}

void Token_processer_mtllib::process(vector<string>& words, Scene& scene) const
{
	if (words.size() != 2)
	{
		printf("wrong line in file\n");
		return;
	}
	scene.Load_Material(words[1]);
}

void Token_processer_usemtl::process(vector<string>& words, Scene & scene) const
{
	scene.triangle_bsdf = scene.materials[words[1]];
}

void read_face_helper(const vector<string>& words, int idx, size_t& v, size_t& vn)
{
	vector<string> face;
	SplitString(words[idx], face, "/");
	stringToNum(v, face[0]); stringToNum(vn, face[2]);
}

void Token_processer_face::process(vector<string>& words, Scene& scene) const
{
	if (words.size() < 4)
	{
		printf("wrong line in file\n");
		return;
	}
	size_t v0, v1, v2;
	size_t vn0, vn1, vn2;
	read_face_helper(words, 1, v0, vn0);
	read_face_helper(words, 2, v1, vn1);
	for (int i = 3; i < words.size(); i++)
	{
		read_face_helper(words, i, v2, vn2);
		Triangle *p = new Triangle(v0 - 1, v1 - 1, v2 - 1, vn0 - 1, vn1 - 1, vn2 - 1, scene.triangle_bsdf, &scene.mesh);
		printf("reading f %d/%d %d/%d %d/%d\n", p->v0, p->n0, p->v1, p->n1, p->v2, p->n2);
		scene.primitives.push_back(p);
		v1 = v2; vn1 = vn2;
	}
}

void Scene::Process_token(vector<string>& words)
{
	string token = words[0];
	auto it = token_processers.find(token);
	if (it != token_processers.end())
	{
		it->second->process(words, *this);
	}
	else
	{
		printf("not implement yet\n");
	}
}

void Scene::Load_Scene(string mesh_file_name, string light_file_name)
{
	Load_Light(light_file_name);
	ifstream mesh_file;
	mesh_file.open(mesh_file_name);
	if (!mesh_file.is_open())
	{
		printf("can not open file %s\n", mesh_file_name.c_str());
		return;
	}
	else
	{
		size_t pos = mesh_file_name.find_last_of("/");
		prefix = mesh_file_name.substr(0, pos + 1);
		printf("prefix = %s\n", prefix.c_str());
		string line;
		while (getline(mesh_file, line))
		{
			printf("read line %s\n", line.c_str());
			vector<string> words;
			SplitString(line, words, " ");
			if (words.size() < 1)
				continue;
			string token = words[0];
			Process_token(words);
		}
	}
}

void Scene::Print_Mesh()
{
	printf("printf Scene contents:\n");
	printf("vertex coordinate:\n");
	for (auto e : materials)
	{
		printf("\nmaterial name is %s:\n", e.first.c_str());
		e.second->print_bsdf();
	}
	for (auto v : mesh.vertices)
	{
		printf("v %lf %lf %lf\n", v.x, v.y, v.z);
	}
	printf("vertex normal:\n");
	for (auto v : mesh.normals)
	{
		printf("vn %lf %lf %lf\n", v.x, v.y, v.z);
	}
	printf("primitives:\n");
	for (auto p : primitives)
	{
		printf("f %d/%d %d/%d %d/%d\n", p->v0, p->n0, p->v1, p->n1, p->v2, p->n2);
		p->get_bsdf()->print_bsdf();
	}
	return;
}

void Scene::Load_Light(string light_file_name)
{
	printf("Load Lights from %s\n", light_file_name.c_str());
}

void Scene::Load_Material(string material_file_name)
{
	material_file_name = prefix + material_file_name;
	printf("Load Materials from %s\n", material_file_name.c_str());
	ifstream material_file(material_file_name);
	if (!material_file.is_open())
	{
		printf("can not open file %s\n", material_file_name.c_str());
		return;
	}
	else
	{
		string line;
		int state = 0; //0 wating the matial name, 1 ready to read params
		string name("");
		Color Kd;
		Color Ka;
		Color Ks;
		Color Tf;
		double Ni = 1.0;
		double Ns = 0;
		while (getline(material_file, line))
		{
			vector<string> words;
			SplitString(line, words, " ");
			if (words[0] == "newmtl")
			{
				if(name == "")
					name = words[1];
				else
				{
					BlinnPhonBSDF * newmtl = new BlinnPhonBSDF(name, Ka, Kd, Ks, Tf, Ni, Ns);
					materials[name] = newmtl;
					Kd = Color(0, 0, 0);
					Ka = Color(0, 0, 0);
					Ks = Color(0, 0, 0);
					Tf = Color(0, 0, 0);
					Ni = 1.0;
					Ns = 0;
					name = words[1];
				}
			}
			else if (words[0] == "Kd")
			{
				stringToNum(Kd.r, words[1]);
				stringToNum(Kd.g, words[2]);
				stringToNum(Kd.b, words[3]);
			}
			else if (words[0] == "Ka")
			{
				stringToNum(Ka.r, words[1]);
				stringToNum(Ka.g, words[2]);
				stringToNum(Ka.b, words[3]);
			}
			else if (words[0] == "Ks")
			{
				stringToNum(Ks.r, words[1]);
				stringToNum(Ks.g, words[2]);
				stringToNum(Ks.b, words[3]);
			}
			else if (words[0] == "Tf")
			{
				stringToNum(Tf.r, words[1]);
				stringToNum(Tf.g, words[2]);
				stringToNum(Tf.b, words[3]);
			}
			else if (words[0] == "Ni")
			{
				stringToNum(Ni, words[1]);
			}
			else if (words[0] == "Ns")
			{
				stringToNum(Ns, words[1]);
			}
		}
	}
}

