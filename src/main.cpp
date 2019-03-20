#include "scene/mesh.h"

int main()
{
	Mesh mesh;
	mesh.Load_Scene(string("D:/backup for c disk/study/ZJU_CG/ray_tracer/scene/SceneDebug/simple.obj"), string("22"));
	mesh.Print_Mesh();
}