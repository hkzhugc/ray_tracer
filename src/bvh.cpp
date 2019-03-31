#include "bvh.h"

void BVH::build_bvh(const vector<Triangle*>& triangles)
{
	printf("building bvh\n");
	root = construct_bvh(triangles);
	printf("building bvh done\n");
}

//helper func to compute the centroid in which bucket
int compute_bucket(double centroid, double min_val, double max_val, int bucket_num)
{
	double idx;
	idx = ((centroid - min_val) / (max_val - min_val)) * bucket_num;
	idx = idx >= bucket_num ? bucket_num - 1 : idx;
	return (int)idx;
}

//compare func, use centroid to sort the triangle
bool triangleCompare(const Triangle *p1, const Triangle *p2, int split_axis)
{
	double v1, v2;
	v1 = p1->get_bbox().centroid()[split_axis];
	v2 = p2->get_bbox().centroid()[split_axis];
	return v1 < v2;
}

BVHNode* BVH::construct_bvh(const std::vector<Triangle*>& triangles) {

#define max_leaf_size 4

	BBox centroid_box, bbox;
	bool use_sah = true;

	//compute the bbox tris
	for (Triangle *p : triangles) {
		BBox bb = p->get_bbox();
		bbox.expand(bb);
		Vector3D c = bb.centroid();
		centroid_box.expand(c);
	}

	BVHNode *node = new BVHNode(bbox);

	if (triangles.size() <= max_leaf_size)
	{
		node->triangles = new vector<Triangle *>(triangles);
		return node;
	}
	else //bvh use sah
	{
		int split_axis = 0;
		double max_extent = centroid_box.extent[0];
		for (int i = 1; i < 3; i++)
		{
			if (max_extent < centroid_box.extent[i])
			{
				max_extent = centroid_box.extent[i];
				split_axis = i;
			}
		}
		if (use_sah) {
#define Bucket_num 64
			BBoxBucket bboxBucket[Bucket_num];
			BBoxBucket bboxBucketPreSum[Bucket_num];
			BBoxBucket bboxBucketRevSum[Bucket_num];
			double min_val = bbox.min[split_axis];
			double max_val = bbox.max[split_axis];
			int idx;
			for (Triangle *p : triangles)
			{
				idx = compute_bucket(p->get_bbox().centroid()[split_axis], min_val, max_val, Bucket_num);
				//cout << endl << "cal idx : " << idx << endl;
				bboxBucket[idx].expand(p);
			}
			bboxBucketPreSum[0].expand(bboxBucket[0]);
			bboxBucketRevSum[Bucket_num - 1].expand(bboxBucket[Bucket_num - 1]);
			for (int i = 1; i < Bucket_num; i++)
			{
				bboxBucketPreSum[i].expand(bboxBucket[i]);
				bboxBucketPreSum[i].expand(bboxBucketPreSum[i - 1]);
				//cout << "Pre Size " << i << " : " << bboxBucketPreSum[i].p_list.size() << endl;
				bboxBucketRevSum[Bucket_num - i - 1].expand(bboxBucket[Bucket_num - i - 1]);
				bboxBucketRevSum[Bucket_num - i - 1].expand(bboxBucketRevSum[Bucket_num - i]);
				//cout << "Rev Size " << bucket_num - i - 1 << " : " << bboxBucketRevSum[bucket_num - i - 1].p_list.size() << endl;
			}
			double max_cost = (double)triangles.size() * bbox.surface_area();
			int partition_num = Bucket_num - 1;
			for (int i = 0; i < Bucket_num - 1; i++)
			{
				double this_cost = bboxBucketPreSum[i].get_cost() + bboxBucketRevSum[i + 1].get_cost();
				if (this_cost < max_cost)
				{
					partition_num = i;
					max_cost = this_cost;
				}
			}
			if (partition_num == Bucket_num - 1)//partition doesnt help
			{
				//node->prims = new vector<Primitive *>(prims);
				//return node;
				size_t mid_element_idx = triangles.size() / 2;
				auto cp_prims = vector<Triangle *>(triangles);
				auto compare_func = [split_axis](Triangle *p1, Triangle *p2) {return triangleCompare(p1, p2, split_axis); };
				std::nth_element(cp_prims.begin(), cp_prims.begin() + mid_element_idx, cp_prims.end(), compare_func);
				vector<Triangle *> left = vector<Triangle *>(cp_prims.begin(), cp_prims.begin() + mid_element_idx);
				vector<Triangle *> right = vector<Triangle *>(cp_prims.begin() + mid_element_idx, cp_prims.end());
				node->l = construct_bvh(left);
				node->r = construct_bvh(right);
				return node;
			}
			else
			{
				//cout << "left size : " << bboxBucketPreSum[partition_num].p_list.size() << endl;
				//cout << "right size : " << bboxBucketRevSum[partition_num + 1].p_list.size() << endl;
				node->l = construct_bvh(bboxBucketPreSum[partition_num].p_list);
				node->r = construct_bvh(bboxBucketRevSum[partition_num + 1].p_list);
				return node;
			}
		}
		else// use mid element partition
		{
			size_t mid_element_idx = triangles.size() / 2;
			auto cp_prims = vector<Triangle *>(triangles);
			auto compare_func = [split_axis](Triangle *p1, Triangle *p2) {return triangleCompare(p1, p2, split_axis); };
			std::nth_element(cp_prims.begin(), cp_prims.begin() + mid_element_idx, cp_prims.end(), compare_func);
			vector<Triangle *> left = vector<Triangle *>(cp_prims.begin(), cp_prims.begin() + mid_element_idx);
			vector<Triangle *> right = vector<Triangle *>(cp_prims.begin() + mid_element_idx, cp_prims.end());
			node->l = construct_bvh(left);
			node->r = construct_bvh(right);
			return node;
		}
	}
	return node;
}

bool BVH::intersect(Ray& ray, BVHNode *node) const {

	bool hit = false;
	double min_t = ray.min_t;
	double max_t = ray.max_t;
	if (!node->bb.intersect(ray, min_t, max_t))
		return hit;
	if (node->isLeaf())
	{
		for (Triangle *p : *(node->triangles)) {
			if (p->intersect(ray))
				return true;
		}
		return hit;
	}
	hit |= intersect(ray, node->l);
	if (hit) return hit;
	hit |= intersect(ray, node->r);
	return hit;
}

bool BVH::intersect(Ray& ray, Intersection* i, BVHNode *node) const 
{
	bool hit = false;
	double min_t = ray.min_t;
	double max_t = ray.max_t;
	if (!node->bb.intersect(ray, min_t, max_t))
		return hit;
	if (node->isLeaf())
	{
		for (Triangle *p : *(node->triangles)) {
			if (p->intersect(ray, i))
				hit = true;
		}
		return hit;
	}
	hit |= intersect(ray, i, node->l);
	hit |= intersect(ray, i, node->r);
	return hit;
}