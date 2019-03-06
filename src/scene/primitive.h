#pragma once

#include "../BBox.h"
#include "../ray.h"
#include "../intersection.h"
#include "../BSDF.h"

//定义一个抽象的基类，提供几何体的一些基本操作接口
class Primitive
{
public:
	//用来返回几何体的AABB，用来BVH的构建对光线追踪进行加速
	virtual BBox get_bbox() const = 0;

	virtual void intersect(Ray &r) const = 0;

	virtual void intersect(Ray &r, Intersection &i) const = 0;

	virtual BSDF* get_bsdf() const = 0;
	 
private:

};