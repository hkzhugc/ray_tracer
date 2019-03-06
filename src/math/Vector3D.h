#pragma once
#include <cmath>
//定义自己使用的Vector
class Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0) {}
	Vector3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	Vector3D(double c) : x(c), y(c), z(c) {}
	Vector3D(const Vector3D &v) : x(v.x), y(v.y), z(v.z) {}
	~Vector3D();

	double x, y, z;

	inline double& operator[] (const int& index) {
		return (&x)[index];
	}

	inline const double& operator[] (const int& index) const {
		return (&x)[index];
	}
	
	inline bool operator== (const Vector3D & v) const {
		return (x == v.x && y == v.y && z == v.z);
	}

	inline Vector3D operator-(void) const {
		return Vector3D(-x, -y, -z);
	}

	inline Vector3D operator+(const Vector3D& v) const {
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3D operator-(const Vector3D& v) const {
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3D operator*(const double& c) const {
		return Vector3D(x * c, y * c, z * c);
	}

	inline Vector3D operator/(const double& c) const {
		const double rc = 1.0 / c;
		return Vector3D(rc * x, rc * y, rc * z);
	}

	inline void operator+=(const Vector3D& v) {
		x += v.x; y += v.y; z += v.z;
	}

	inline void operator-=(const Vector3D& v) {
		x -= v.x; y -= v.y; z -= v.z;
	}

	inline void operator*=(const double& c) {
		x *= c; y *= c; z *= c;
	}

	inline void operator/=(const double& c) {
		(*this) *= (1. / c);
	}

	//返回向量的长度
	inline double norm(void) const {
		return sqrt(x*x + y*y + z*z);
	}

	//返回向量长度的平方
	inline double norm2(void) const {
		return x*x + y*y + z*z;
	}

	//返回向量的单位向量
	inline Vector3D unit(void) const {
		double rNorm = 1. / sqrt(x*x + y*y + z*z);
		return Vector3D(rNorm*x, rNorm*y, rNorm*z);
	}

	//单位化向量
	inline void normalize(void) {
		(*this) /= norm();
	}

};

//定义左乘一个标量
inline Vector3D operator* (const double& c, const Vector3D& v) {
	return Vector3D(c * v.x, c * v.y, c * v.z);
}

//定义点积
inline double dot(const Vector3D& u, const Vector3D& v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

//定义叉乘
inline Vector3D cross(const Vector3D& u, const Vector3D& v) {
	return Vector3D(u.y*v.z - u.z*v.y,
		u.z*v.x - u.x*v.z,
		u.x*v.y - u.y*v.x);
}

