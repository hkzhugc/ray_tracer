#pragma once
#include "scene/scene.h"
#include "mypng/myypng.h"
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
		:camera(Vector3D(0, 0, 0.4), Vector3D(), Vector3D(0, 1, 0), 100.f, Width, Height), png(Width, Height)
	{
		screenW = Width;
		screenH = Height;
	}
	~ray_tracer() {}

	void init_scene(string mesh_file, string light_file);
	Color trace_ray(Ray& r);
	Color trace_pixel(size_t x, size_t y);
	void trace_scene();
private:
	Color estimate_direct_light(const Ray& r, const Intersection& i);


	myypng png;
	Scene scene;
	Camera camera;
	size_t screenW, screenH;
	vector<Vector3D> intersec_ps;
};

