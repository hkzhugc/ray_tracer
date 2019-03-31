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

	ray_tracer() 
		//://camera(Vector3D(0, 0, 4), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 50.f, Width, Height), png(Width, Height), png_hot(Width, Height)//Scene01
		//camera(Vector3D(0.0, 0.64, 0.52), Vector3D(0.0, 0.40, 0.3), Vector3D(0, 1, 0), 60.f, Width, Height), png(Width, Height), png_hot(Width, Height)//Scene02
		//camera(Vector3D(0.0, 2.0, 15.0), Vector3D(0.0, 1.69521, 14.0476), Vector3D(0.0, 0.952421, -0.304787), 28.f, Width, Height), png(Width, Height), png_hot(Width, Height) //Scene03
	{
		ns_lights = 5;
		ns_pixel = 5;
		trans_parent_cnt = 0;
		trans_parent_cnt_L = 0;
	}
	~ray_tracer() {}

	bool init_scene(string mesh_file, string light_file);
	void Load_camera(string camera_name, Vector3D pos, Vector3D look_at, Vector3D up, float fovy, size_t screenW, size_t screenH);
	void Load_camera(string camera_name, const Camera& cam);
	bool Set_working_camera(string camera_name);
	void Set_sample_time(size_t _ns_lights, size_t _ns_pixel) { ns_lights = _ns_lights; ns_pixel = _ns_pixel; }
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
	map<string, Camera> cameras;
	Camera working_cam;
	size_t screenW, screenH;
	vector<Vector3D> intersec_ps;
	FILE * intersect_file;

	int trans_parent_cnt;
	int trans_parent_cnt_L;

	size_t ns_lights;
	size_t ns_pixel;

	string output_name;
};

