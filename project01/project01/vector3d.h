#pragma once
#include <math.h>

class Vector3d{
	public:
		Vector3d(){}
		Vector3d(float _x, float _y, float _z):x(_x),y(_y),z(_z){}
		float x,y,z;
		friend Vector3d operator/(const Vector3d &p, float a);
		friend Vector3d operator+(const Vector3d &a, const Vector3d &b);
		friend Vector3d operator-(const Vector3d &a, const Vector3d &b);
		friend Vector3d operator*(const Vector3d &p, float a);
		friend float operator*(const Vector3d &a, const Vector3d &b);
		float length();
		Vector3d normalize();
};

Vector3d operator/(const Vector3d &p, float a)
{
    return Vector3d(p.x/a,p.y/a,p.z/a);
}

Vector3d operator+(const Vector3d &a, const Vector3d &b)
{
    return Vector3d(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vector3d operator-(const Vector3d &a, const Vector3d &b)
{
    return Vector3d(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vector3d operator*(const Vector3d &p, float a)
{
    return Vector3d(p.x*a,p.y*a,p.z*a);
}

float operator*(const Vector3d &a, const Vector3d &b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

float Vector3d::length(){
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

Vector3d Vector3d::normalize(){
	return Vector3d(x/length(),y/length(),z/length());
}
