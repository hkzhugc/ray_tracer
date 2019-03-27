#include "ray_tracer.h"

void ray_tracer::init_scene(string mesh_file, string light_file)
{
	scene.Load_Scene(mesh_file, light_file);
}

//TODO should check the ray is a camera ray or a ray reflect by sample specular
Color ray_tracer::trace_ray(Ray &r)
{
	Intersection intersect;
	Color light_color;

	//TODO : should be replaced by bvh->intersect
	bool flag = false;
	for (auto p : scene.primitives)
	{
		bool temp = p->intersect(r, &intersect);
		flag |= temp;
	}

	if (!flag){
		printf("no intersect, r pos (%lf, %lf, %lf)\n", r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
		return Color(0, 0, 0);
	}


	//TODO : if ray is a camera ray or a ray reflect by sample specular
	//       should add the intersect materials's emisson


	//sample the direct light, 
	light_color += estimate_direct_light(r, intersect);

	//TODO : sample indirect light, acoording to the depth of the ray
	if (r.depth > 0)
		light_color += estimate_indirect_light(r, intersect);

	return light_color;

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
			Color pixel_color;
			for (size_t i = 0; i < ns_pixel; i++)
				pixel_color += trace_pixel(x, y);
			pixel_color /= ns_pixel;
			png.setImagePixel(x, y, pixel_color);
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

Color ray_tracer::estimate_direct_light(const Ray & r, const Intersection & intersect)
{
	Matrix3x3 o2w;
	make_coord_space(o2w, intersect.n);
	Matrix3x3 w2o = o2w.T();

	const Vector3D& hit_p = r.at_time(intersect.t);
	const Vector3D& w_out = (w2o * (-r.d)).unit();

	Color L_out;
	for (auto l : scene.lights)
	{
		Color light_contribution;
		size_t sample_times = ns_lights; //TODO : check if it is a delta light, then it should sample only once
		for (size_t i = 0; i < sample_times; i++)
		{
			Vector3D wi; //in world coord
			Vector3D w_in; //in local coord
			float light_distance;
			float pdf;

			Color sample_light = l->sample_L(hit_p, &wi, &light_distance, &pdf);
			if (sample_light.r < 0 || sample_light.g < 0 || sample_light.b < 0)
			{
				printf("wrong when sample light\n");
				getchar();
			}

			w_in = w2o * wi;
			if (w_in.z < 0) // light is behind the obj
				continue;

			Vector3D ray_pos = hit_p + EPS_D * wi;
			Ray shadow_ray(ray_pos, wi);
			shadow_ray.max_t = light_distance;

			//TODO : should be replace by bvh->intersect
			bool is_block = false;
			for (auto p : scene.primitives)
			{
				is_block |= p->intersect(shadow_ray);
				if (is_block) break;
			}

			if (!is_block)
				//rendering equation  L_out =   f                      *  L           * cos / pdf ?
				light_contribution += intersect.bsdf->f(w_out, w_in) * sample_light * fabs(w_in.z);
		}
		L_out += (light_contribution / sample_times);
	}

	return L_out;
}

Color ray_tracer::estimate_indirect_light(const Ray& r, const Intersection& intersect)
{
	Matrix3x3 o2w;
	make_coord_space(o2w, intersect.n);
	Matrix3x3 w2o = o2w.T();

	const Vector3D& hit_p = r.at_time(intersect.t);
	const Vector3D& w_out = w2o * (-r.d);

	Color L_out;

	Vector3D wi; //in world coord
	Vector3D w_in; //in local coord
	float pdf;

	Color reflectance = intersect.bsdf->sample_f(w_out, &w_in, &pdf);

	if (w_in.z < 0)
		printf("when indirect light, sample a neg reflect\n");

	wi = o2w * w_in;
	wi.normalize();

	Vector3D ray_pos = hit_p + EPS_D * wi;
	Vector3D delta = ray_pos - hit_p;
	Ray next_ray(ray_pos, wi);
	next_ray.depth = r.depth - 1;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (fabs(ray_pos[i]) > (3.0 + EPS_D)){
	//		printf("wrong at compute the hit_p(%lf %lf %lf)\n", ray_pos[0], ray_pos[1], ray_pos[2]);
	//		printf("delta (%lf %lf %lf) wi (%lf %lf %lf)\n", delta[0], delta[1], delta[2], wi[0], wi[1], wi[2]);
	//		getchar();
	//	}
	//}

	L_out = reflectance * trace_ray(next_ray) * fabs(w_in.z) / pdf;
	return L_out;
}
