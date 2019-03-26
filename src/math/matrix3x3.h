#pragma once
#include "Vector3D.h"

struct Matrix3x3
{
	Matrix3x3() {};
	Matrix3x3(double * data) {
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				(*this)(i, j) = data[3 * i + j];
		}
	}

	//col
	Vector3D entries[3];

	double& operator()(int i, int j)
	{
		return entries[j][i];
	}

	const double& operator()(int i, int j) const
	{
		return entries[j][i];
	}

	Vector3D operator*(const Vector3D& x) const
	{
		return x[0] * entries[0] 
			+ x[1] * entries[1]
			+ x[2] * entries[2];
	}

	Vector3D& operator[] (int idx)
	{
		return entries[idx];
	}

	const Vector3D& operator[] (int idx) const
	{
		return entries[idx];
	}

	Matrix3x3 T(void) const {
		const Matrix3x3& A(*this);
		Matrix3x3 B;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				B(i, j) = A(j, i);
			}
		return B;
	}
};