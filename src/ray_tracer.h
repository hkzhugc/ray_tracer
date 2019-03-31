#pragma once
#include "scene/scene.h"
#include "mypng/myypng.h"
#include "bvh.h"
#include "Camera.h"
#define DEBUG
#define Width 512
#define Height 512
#define EPS_D 1e-8
class ray_tracer
{
public:

	//TODO add flexible way to init the camera and the png
	ray_tracer() 
		:camera(Vector3D(0, 0, 4), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 50.f, Width, Height), png(Width, Height), png_hot(Width, Height)//Scene01
		//camera(Vector3D(0.0, 2.0, 15.0), Vector3D(0.0, 1.69521, 14.0476), Vector3D(0.0, 0.952421, -0.304787), 28.f, Width, Height), png(Width, Height), png_hot(Width, Height) //Scene03
	{
		screenW = Width;
		screenH = Height;
		ns_lights = 5;
		ns_pixel = 5000;
		trans_parent_cnt = 0;
		trans_parent_cnt_L = 0;
	}
	~ray_tracer() {}

	void init_scene(string mesh_file, string light_file);
	Color trace_ray(Ray& r, bool is_delta_light = false);
	Color trace_pixel(size_t x, size_t y);
	void trace_scene();
private:
	Color estimate_direct_light(const Ray& r, const Intersection& intersect);
	Color estimate_indirect_light(const Ray& r, const Intersection& intersect);

	BVH bvh;
	myypng png;
	myypng png_hot;
	Scene scene;
	Camera camera;
	size_t screenW, screenH;
	vector<Vector3D> intersec_ps;
	FILE * intersect_file;

	int trans_parent_cnt;
	int trans_parent_cnt_L;

	size_t ns_lights;
	size_t ns_pixel;
};

