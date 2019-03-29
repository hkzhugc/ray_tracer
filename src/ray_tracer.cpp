#include "ray_tracer.h"

void ray_tracer::init_scene(string mesh_file, string light_file)
{
	scene.Load_Scene(mesh_file, light_file);
	scene.Print_Mesh();
}

Color ray_tracer::trace_ray(Ray &r, bool is_delta_light)
{
	Intersection intersect;
	Color light_color;

	bool flag = false;
	flag = bvh.intersect(r, &intersect);
	
	if (!flag){
		return Color(0, 0, 0);
	}

	if (is_delta_light)
		light_color += intersect.bsdf->get_emission();

	if (isnan(light_color.r) && isnan(light_color.g) && isnan(light_color.b))
	{
		printf("before sample direct isnan ray pos %f %f %f\n", r.depth, r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}
	//sample the direct light
	Color direct_light = estimate_direct_light(r, intersect);
	Color temp_c = light_color;
	light_color += direct_light;

	if (isnan(light_color.r) && isnan(light_color.g) && isnan(light_color.b))
	{
		printf("after sample direct before indir isnan ray pos %f %f %f\n", r.depth, r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}

	//TODO : sample indirect light, acoording to the depth of the ray
	Color indirect_light;
	if (r.depth > 0) 
	{
		indirect_light = estimate_indirect_light(r, intersect);
		light_color += indirect_light;
	}

	if (isnan(light_color.r) && isnan(light_color.g) && isnan(light_color.b))
	{
		printf("isnan trace ray pos %f %f %f\n", r.depth, r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}

	return light_color;

}

Color ray_tracer::trace_pixel(size_t x, size_t y)
{
	double delta_x = /*0.5; */random_uniform();
	double delta_y = /*0.5; */random_uniform();
	Ray ray = camera.generate_ray((x + delta_x) / screenW, (y + delta_y) / screenH);
	return trace_ray(ray, true);
}

void ray_tracer::trace_scene()
{
	bvh.build_bvh(scene.primitives);
	int process_percent = 0;
	int row_cnt = 0;
#pragma omp parallel for
	for (int x = 0; x < screenW; x++)
	{
		for (int y = 0; y < screenH; y++)
		{
			Color pixel_color;
			for (int i = 0; i < ns_pixel; i++) {
				Color res = trace_pixel(x, y);
				if (isnan(res.r) && isnan(res.g) && isnan(res.b))
				{
					printf("is not a num %d %d %d\n", x, y, i);
				}
				pixel_color += res;
			}
			pixel_color /= ns_pixel;
			pixel_color.gama();
			png.setImagePixel(x, y, pixel_color);
		}
		row_cnt++;
		if (row_cnt * 100 / screenW > process_percent) {
			printf("process_percent : %4d%\n", process_percent);
			process_percent = row_cnt * 100 / screenW;
		}
	}
#ifdef DEBUG
	//FILE * intersect_file;
	//
	//for (auto v : intersec_ps)
	//{
	//	fprintf_s(intersect_file, "v %f %f %f\n", v.x, v.y, v.z);
	//}
	//fclose(intersect_file);
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
		size_t sample_times = ns_lights;
		for (size_t i = 0; i < sample_times; i++)
		{
			Vector3D wi; //in world coord
			Vector3D w_in; //in local coord
			double light_distance;
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

			if (w_in.z < EPS_D)
				continue;

			Vector3D ray_pos = hit_p + EPS_D * wi;
			Ray shadow_ray(ray_pos, wi);
			shadow_ray.max_t = light_distance - EPS_D;

			bool is_block = false;
			is_block = bvh.intersect(shadow_ray);

			if (!is_block) 
			{
				//rendering equation  L_out =   f                      *  L           * cos / pdf ?
				Color c = intersect.bsdf->f(w_out, w_in) * sample_light * fabs(w_in.z) / pdf;
				light_contribution += c;
			}
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

	bool is_sample_specular = false;
	Color reflectance = intersect.bsdf->sample_f(w_out, &w_in, &pdf, &is_sample_specular);

	//if it 
	if (w_in.z < EPS_D || pdf < EPS_D)
		return L_out;

	wi = o2w * w_in;
	wi.normalize();

	Vector3D ray_pos = hit_p + EPS_D * wi;
	Ray next_ray(ray_pos, wi);
	next_ray.depth = r.depth - 1;

	L_out = reflectance * trace_ray(next_ray, true) * fabs(w_in.z) / pdf;

	if (isnan(L_out.r) && isnan(L_out.g) && isnan(L_out.b))
	{
		printf("isnan estimate_indirect_light ray pos %f %f %f\n", r.depth, r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
		printf("isnan pdf  %f w_in.z = %f, is_sample_specular = %d\n", pdf, w_in.z, is_sample_specular);
		getchar();
	}

	return L_out;
}
