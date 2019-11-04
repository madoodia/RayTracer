// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef vec3_H
#define vec3_H

#include <math.h>
#include <stdlib.h>


class vec3
{
public:
	vec3() {}
	vec3(float x, float y, float z) { X = x; Y = y; Z = z; }

	float x() const { return X; }
	float y() const { return Y; }
	float z() const { return Z; }

	const vec3& operator+() const { return *this; }
	vec3 operator+(const vec3& v1) const { return vec3(x() + v1.x(), y() + v1.y(), z() + v1.z()); }

	vec3 operator-() const { return vec3(-x(), -y(), -z()); }
	vec3 operator-(const vec3& v1)const { return vec3(x() - v1.x(), y() - v1.y(), z() - v1.z()); }

	vec3 operator*(const vec3& v1) const { return vec3(x() * v1.x(), y() * v1.y(), z() * v1.z()); }
	vec3 operator*(float scalar) const { return vec3(x() * scalar, y() * scalar, z() * scalar); }

	vec3 operator/(const vec3& v1) const { return vec3(x() / v1.x(), y() / v1.y(), z() / v1.z()); }
	vec3 operator/(float scalar) const { return vec3(x() / scalar, y() / scalar, z() / scalar); }

	vec3 operator+=(const vec3& v2) const { return *this + v2; }
	vec3 operator-=(const vec3& v2) const { return *this - v2; }
	vec3 operator*=(const vec3& v2) const { return *this * v2; }
	vec3 operator/=(const vec3& v2) const { return *this / v2; }
	vec3 operator*=(const float t) const { return *this * t; }
	vec3 operator/=(const float t) const { return *this / t; }

	float length() const { return sqrt(x() * x() + y() * y() + z() * z()); }
	float squareLength() const { return x() * x() + y() * y() + z() * z(); }
	vec3 normalize() { return *this / length(); }

	float dot(const vec3& v1, const vec3& v2) { return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z(); }
	vec3 cross(const vec3& v1, const vec3& v2)
	{
		return vec3((v1.y() * v2.z() - v1.z() * v2.y()),
			-(v1.x() * v2.z() - v1.z() * v2.x()),
			(v1.x() * v2.y() - v1.y() * v2.x()));
	}

	friend std::ostream& operator<<(std::ostream& stream, const vec3& other)
	{
		stream << other.x() << " " << other.y() << " " << other.z() << std::endl;
		return stream;
	}

private:
	float X, Y, Z;
};



#endif // vec3_H