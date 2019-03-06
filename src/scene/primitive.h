#pragma once

#include "../BBox.h"
#include "../ray.h"
#include "../intersection.h"
#include "../BSDF.h"

//����һ������Ļ��࣬�ṩ�������һЩ���������ӿ�
class Primitive
{
public:
	//�������ؼ������AABB������BVH�Ĺ����Թ���׷�ٽ��м���
	virtual BBox get_bbox() const = 0;

	virtual void intersect(Ray &r) const = 0;

	virtual void intersect(Ray &r, Intersection &i) const = 0;

	virtual BSDF* get_bsdf() const = 0;
	 
private:

};