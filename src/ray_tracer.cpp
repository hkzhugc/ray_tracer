#include "ray_tracer.h"

bool ray_tracer::init_scene(string mesh_file, string light_file)
{
	bool flag = scene.Load_Scene(mesh_file, light_file);
	scene.Print_Mesh();

	int pos1, pos2;
	pos1 = mesh_file.find_last_of("/\\") + 1;
	pos2 = mesh_file.find_last_of(".");
	output_name = mesh_file.substr(pos1, pos2 - pos1);

	return flag;
}

void ray_tracer::Load_camera(string camera_name, Vector3D pos, Vector3D look_at, Vector3D up, float fovy, size_t screenW, size_t screenH)
{
	Camera cam(pos, look_at, up, fovy, screenW, screenH);
	Load_camera(camera_name, cam);
}

void ray_tracer::Load_camera(string camera_name, const Camera & cam)
{
	cameras[camera_name] = cam;
}

bool ray_tracer::Set_working_camera(string camera_name)
{
	auto it = cameras.find(camera_name);
	if (it != cameras.end())
	{
		working_cam = (it->second);
		screenW = working_cam.get_screenW();
		screenH = working_cam.get_screenH();
		png.init_png(screenW, screenH);
		png_hot.init_png(screenW, screenH);
		return true;
	}
	else
	{
		printf("cannot set camera %s\n", camera_name.c_str());
		return false;
	}
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
		printf("before sample direct isnan ray pos %f %f %f\n", r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}
	//sample the direct light
	Color direct_light = estimate_direct_light(r, intersect);
	Color temp_c = light_color;
	light_color += direct_light;

	if (isnan(light_color.r) && isnan(light_color.g) && isnan(light_color.b))
	{
		printf("after sample direct before indir isnan ray pos %f %f %f\n", r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}

	Color indirect_light;
	if (r.depth > 0) 
	{
		indirect_light = estimate_indirect_light(r, intersect);
		light_color += indirect_light;
	}

	if (isnan(light_color.r) && isnan(light_color.g) && isnan(light_color.b))
	{
		printf("isnan trace ray pos %f %f %f\n", r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
	}

	return light_color;

}

Color ray_tracer::trace_pixel(size_t x, size_t y)
{
	double delta_x = /*0.5; */random_uniform();
	double delta_y = /*0.5; */random_uniform();
	if (true)
	{
		Ray ray = working_cam.generate_ray((x + delta_x) / screenW, (y + delta_y) / screenH);
		return trace_ray(ray, true);
	}
	return Color();
}

void ray_tracer::trace_scene()
{
	bvh.build_bvh(scene.primitives);
	clock_t start = clock();
	int process_percent = 0;
	int row_cnt = 0;
	double maxTolerance = 0.05;
#define samplesPerBatch 10
#pragma omp parallel for
	for (int x = 0; x < screenW; x++)
	{
		for (int y = 0; y < screenH; y++)
		{
			Color pixel_color;
			double s1 = 0, s2 = 0;
			int i = 1;
			for (; i <= ns_pixel; i++) {
				Color res = trace_pixel(x, y);
				pixel_color += res;
				double illum = res.illum();
				s1 += illum;
				s2 += illum * illum;
				if (i % samplesPerBatch == 0)
				{
					double var = (1. / (i - 1)) * (s2 - s1 * s1 / i);
					double CI = 1.96 * sqrt(var / i);
					//printf("CI = %lf, var = %lf, s1 / i = %lf, maxTolerance * (s1 / i) : %lf", CI, var, s1 / i, maxTolerance * (s1 / i));
					if (CI <= maxTolerance * (s1 / i))
					{
						break;
					}
				}
			}
			pixel_color /= i;
			pixel_color.gama();
			png.setImagePixel(x, y, pixel_color);
			png_hot.setImagePixel(x, y, Color((float)i / (float)ns_pixel, 0.3, 0.3));
		}
		row_cnt++;
		if (row_cnt * 100 / screenW > process_percent) {
			printf("process_percent : %4d\%\n", process_percent);
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
	clock_t end = clock();
	printf("cost %lf min\n", difftime(end, start) / 1000. / 60.);
	printf("trans_parent_cnt = %d, trans_parent_cnt_L = %d\n", trans_parent_cnt, trans_parent_cnt_L);
	png.writeImage((output_name + "_" + std::to_string(ns_lights) + "_" + std::to_string(ns_pixel) + ".png").c_str());
	png_hot.writeImage((output_name + "_" + std::to_string(ns_lights) + "_" + std::to_string(ns_pixel) + "_hot.png").c_str());
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
	if (fabs(w_in.z) < EPS_D || pdf < EPS_D)
		return L_out;

	wi = o2w * w_in;
	wi.normalize();

	Vector3D ray_pos = hit_p + EPS_D * wi;
	Ray next_ray(ray_pos, wi);
	next_ray.depth = r.depth - 1;

	Color L_nextRay = trace_ray(next_ray, is_sample_specular);

	L_out = reflectance * L_nextRay * fabs(w_in.z) / pdf;

	if (w_in.z * w_out.z < 0)
	{
		trans_parent_cnt++;
		if (L_nextRay.illum() > EPS_D) trans_parent_cnt_L++;
		//printf("reflectance %f %f %f\n", reflectance.r, reflectance.g, reflectance.b);
		//printf("L_nextRay %f %f %f\n", L_nextRay.r, L_nextRay.g, L_nextRay.b);
		//printf("w_in.z %lf  L_out %f %f %f\n", w_in.z, L_out.r, L_out.g, L_out.b);
		//getchar();
	}

	if (isnan(L_out.r) && isnan(L_out.g) && isnan(L_out.b))
	{
		printf("isnan estimate_indirect_light ray pos %f %f %f\n", r.at_time(0)[0], r.at_time(0)[1], r.at_time(0)[2]);
		printf("isnan pdf  %f w_in.z = %f, is_sample_specular = %d\n", pdf, w_in.z, is_sample_specular);
		getchar();
	}

	return L_out;
}
