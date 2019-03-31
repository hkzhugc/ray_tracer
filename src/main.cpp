#include "scene/scene.h"
#include "mypng/myypng.h"
#include "ray_tracer.h"
#include <iostream>
int main(int argc, char *argv[])
{
	printf("program working at %s\n", argv[0]);

	ray_tracer rt;
	//load 3 test scene camera
	rt.Load_camera("1", Vector3D(0, 0, 4), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 50.f, 512, 512);
	rt.Load_camera("2", Vector3D(0.0, 0.64, 0.52), Vector3D(0.0, 0.40, 0.3), Vector3D(0, 1, 0), 60.f, 512, 512);
	rt.Load_camera("3", Vector3D(0.0, 2.0, 15.0), Vector3D(0.0, 1.69521, 14.0476), Vector3D(0.0, 0.952421, -0.304787), 28.f, 1152, 864);

	string work_dir(argv[0]);
	string prefix = work_dir.substr(0, work_dir.find_last_of("/\\") + 1);

	printf("please enter the scene file (*.obj)\n");
	string scene_fn;
	string scene_num;
	std::cin >> scene_fn;
	scene_num = scene_fn;
	if (scene_fn == "1")
	{
		scene_fn = "../scene/Scene01/room.obj";
	}
	else if (scene_fn == "2")
	{
		scene_fn = "../scene/Scene02/cup.obj";
	}
	else if (scene_fn == "3")
	{
		scene_fn = "../scene/Scene03/VeachMIS.obj";
	}
	else
	{
		printf("enter the scene num (since, we have no camera or light source file) from 1-2-3\n");
		std::cin >> scene_num;
	}

	printf("enter sample times : sample_light times and sample pixel times\n");
	size_t ns_p, ns_l;
	std::cin >> ns_l >> ns_p;
	rt.Set_sample_time(ns_l, ns_p);

	scene_fn = prefix + scene_fn;
	if(rt.init_scene(string(scene_fn), string(scene_num)) && rt.Set_working_camera(scene_num))
		rt.trace_scene();
}