#include "ray_tracer.h"

void ray_tracer::init_scene(string mesh_file, string light_file)
{
	scene.Load_Scene(mesh_file, light_file);
}

Color ray_tracer::trace_ray(Ray &r)
{
	Intersection intersect;
	bool flag = false;
	for (auto p : scene.primitives)
	{
		bool temp = p->intersect(r, &intersect);
		flag |= temp;
#ifdef DEBUG
		//if (temp)
		//{
		//	intersect.bsdf->print_bsdf();
		//	getchar();
		//}
#endif // DEBUG
	}
	//intersect.bsdf->print_bsdf();
	if (flag)
	{
		intersec_ps.push_back(UniformSphereSampler3D().get_sample());
		//intersec_ps.push_back(r.at_time(intersect.t));
		return intersect.bsdf->Kd;
	}
	else
	{
		return Color(1, 1, 1);
	}
}

Color ray_tracer::trace_pixel(size_t x, size_t y)
{
	Ray ray = camera.generate_ray((x + 0.5) / screenW, (y + 0.5) / screenH);
	return trace_ray(ray);
}

void ray_tracer::trace_scene()
{
	for (size_t x = 0; x < screenW; x++)
	{
		for (size_t y = 0; y < screenH; y++)
		{
			png.setImagePixel(x, y, trace_pixel(x, y));
		}
	}
#ifdef DEBUG
	FILE * intersect_file;
	fopen_s(&intersect_file, "inter_points.obj", "wt");
	for (auto v : intersec_ps)
	{
		fprintf_s(intersect_file, "v %f %f %f\n", v.x, v.y, v.z);
	}
	fclose(intersect_file);
#endif
	png.writeImage("ray_casting.png");
}

Color ray_tracer::estimate_direct_light(const Ray & r, const Intersection & i)
{
	Matrix3x3 o2w;
	make_coord_space(o2w, i.n);
	Matrix3x3 w2o = o2w.T();

	const Vector3D& hit_p = r.at_time(i.t);
	const Vector3D& w_out = w2o * (-r.d);

	Color L_out;
	for (auto l : scene.lights)
	{
		Vector3D wi; //in world coord
		Vector3D w_in; //in local coord
		float light_distance;
		float pdf;

		Color sample_light = l->sample_L(hit_p, &wi, &light_distance, &pdf);

		w_in = w2o * wi;
		if (w_in.z < 0) // light is behind the obj
			continue;

		Ray shadow_ray(hit_p + EPS_D * wi, wi);
		shadow_ray.max_t = light_distance;

		//TODO : should be replace by bvh->intersect
		bool is_block = false;
		for (auto p : scene.primitives)
		{
			is_block |= p->intersect(shadow_ray);
			if (is_block) break;
		}

		if (!is_block)
			L_out += sample_light / pdf; //TODO : should multi by bsdf and cos term
	}

	return Color();
}
