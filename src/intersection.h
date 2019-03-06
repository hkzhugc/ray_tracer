#pragma once
#include "math/Vector3D.h"
#include "BSDF.h"
class Primitive;

class Intersection
{
public:
	Intersection();
	~Intersection();

	double t; //intersect��ʱ��
	Vector3D n; //�����normal
	Primitive* p; //�ཻ�Ķ����
	BSDF* bsdf; //�����bsdf
};

Intersection::Intersection()
{
}

Intersection::~Intersection()
{
}