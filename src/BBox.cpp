#include "BBox.h"

bool BBox::intersect(const Ray& r, double& t0, double& t1) const 
{
	Vector3D tmin, tmax;
	for (int i = 0; i < 3; i++)
	{
		if (r.sign[i])
		{
			tmin[i] = (max[i] - r.o[i]) * r.inv_d[i];
			tmax[i] = (min[i] - r.o[i]) * r.inv_d[i];
		}
		else
		{
			tmin[i] = (min[i] - r.o[i]) * r.inv_d[i];
			tmax[i] = (max[i] - r.o[i]) * r.inv_d[i];
		}
	}
	double min_t = tmin[0];
	double max_t = tmax[0];

	if (min_t > tmax[1] || max_t < tmin[1])
		return false;
	min_t = std::max(min_t, tmin[1]);
	max_t = std::min(max_t, tmax[1]);

	if (min_t > tmax[2] || max_t < tmin[2])
		return false;
	min_t = std::max(min_t, tmin[2]);
	max_t = std::min(max_t, tmax[2]);

	if (min_t > t1 || max_t < t0)
		return false;
	min_t = std::max(min_t, t0);
	max_t = std::min(max_t, t1);

	t0 = min_t;
	t1 = max_t;
	return true;
}
