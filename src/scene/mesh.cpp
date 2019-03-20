#include "mesh.h"

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

void Token_processer_vertex_or_normal::process(vector<string>& words, Mesh& mesh) const
{
	if (words.size() != 4)
	{
		printf("wrong line in file\n");
		return;
	}
	bool isNormal = (words[0] == "vn");
	vector<Vector3D>& tar_vec = (isNormal) ? mesh.vertex_normals : mesh.vertices;
	Vector3D val;
	stringToNum(val.x, words[1]);
	stringToNum(val.y, words[2]);
	stringToNum(val.z, words[3]);
	tar_vec.push_back(val);
}

void Token_processer_mtllib::process(vector<string>& words, Mesh& mesh) const
{
	if (words.size() != 2)
	{
		printf("wrong line in file\n");
		return;
	}
	mesh.Load_Material(words[1]);
}

void Token_processer_face::process(vector<string>& words, Mesh& mesh) const
{
	if (words.size() < 4)
	{
		printf("wrong line in file\n");
		return;
	}
	size_t v0, v1, v2;
	size_t vn0, vn1, vn2;
}

void Mesh::Process_token(vector<string>& words)
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

void Mesh::Load_Scene(string mesh_file_name, string light_file_name)
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

void Mesh::Print_Mesh()
{
	printf("printf Mesh contents:\n");
	printf("vertex coordinate:\n");
	for (auto e : materials)
	{
		printf("\nmaterial name is %s:\n", e.first.c_str());
		e.second->print_bsdf();
	}
	for (auto v : vertices)
	{
		printf("v %lf %lf %lf\n", v.x, v.y, v.z);
	}
	printf("vertex normal:\n");
	for (auto v : vertex_normals)
	{
		printf("vn %lf %lf %lf\n", v.x, v.y, v.z);
	}
	printf("primitives:\n");
	for (auto p : primitives)
	{
		
	}
	return;
}

void Mesh::Load_Light(string light_file_name)
{
	printf("Load Lights from %s\n", light_file_name.c_str());
}

void Mesh::Load_Material(string material_file_name)
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
					BlinnPhonBSDF * newmtl = new BlinnPhonBSDF(Ka, Kd, Ks, Tf, Ni, Ns);
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
