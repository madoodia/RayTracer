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
	union {
		struct
		{
			float x, y, z;
		};
		float e[3];
	};

public:
	vec3() : x(0.0), y(0.0), z(0.0)
	{
	}
	vec3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}

	// Unary Operators
	vec3 &operator-() const
	{
		vec3 r;
		r.x = -x;
		r.y = -y;
		r.z = -z;
		return r;
	}

	// Binary Operators
	vec3 operator+(const vec3 &rhs) const
	{
		vec3 r;
		r.x = x + rhs.x;
		r.y = y + rhs.y;
		r.z = z + rhs.z;
		return r;
	}
	vec3 operator-(const vec3 &rhs) const
	{
		vec3 r;
		r.x = x - rhs.x;
		r.y = y - rhs.y;
		r.z = z - rhs.z;
		return r;
	}
	vec3 operator*(const vec3 &rhs) const
	{
		vec3 r;
		r.x = x * rhs.x;
		r.y = y * rhs.y;
		r.z = z * rhs.z;
		return r;
	}
	vec3 operator/(const vec3 &rhs) const
	{
		vec3 r;
		r.x = x / rhs.x;
		r.y = y / rhs.y;
		r.z = z / rhs.z;
		return r;
	}
	vec3 operator+(float scalar) const
	{
		vec3 r;
		r.x = x + scalar;
		r.y = y + scalar;
		r.z = z + scalar;
		return r;
	}
	vec3 operator-(float scalar) const
	{
		vec3 r;
		r.x = x - scalar;
		r.y = y - scalar;
		r.z = z - scalar;
		return r;
	}
	vec3 operator*(float scalar) const
	{
		vec3 r;
		r.x = x * scalar;
		r.y = y * scalar;
		r.z = z * scalar;
		return r;
	}
	vec3 operator/(float scalar) const
	{
		vec3 r;
		r.x = x / scalar;
		r.y = y / scalar;
		r.z = z / scalar;
		return r;
	}

	// Assignment Operators
	const vec3 &operator+=(const vec3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	const vec3 &operator-=(const vec3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	const vec3 &operator*=(const vec3 &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	const vec3 &operator/=(const vec3 &rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	const vec3 &operator+=(float scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}
	const vec3 &operator-=(float scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}
	const vec3 &operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	const vec3 &operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	inline float operator[](int i) const { return e[i]; };
	inline float &operator[](int i) { return e[i]; };

	float length() const { return sqrt(x * x + y * y + z * z); }
	float squareLength() const { return x * x + y * y + z * z; }
	vec3 normalize() const { return *this / length(); }

	// Friends functions
	friend std::ostream &operator<<(std::ostream &stream, const vec3 &other)
	{
		stream << other.x << " " << other.y << " " << other.z << std::endl;
		return stream;
	}
	friend vec3 operator+(float lhs, const vec3 &rhs) { return rhs + lhs; }
	friend vec3 operator-(float lhs, const vec3 &rhs) { return -(rhs - lhs); }
	friend vec3 operator*(float lhs, const vec3 &rhs) { return rhs * lhs; }
	friend vec3 operator/(float lhs, const vec3 &rhs) { return rhs / lhs; }
};

float dot(const vec3 &v1, const vec3 &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
vec3 cross(const vec3 &v1, const vec3 &v2)
{
	return vec3((v1.y * v2.z - v1.z * v2.y),
				-(v1.x * v2.z - v1.z * v2.x),
				(v1.x * v2.y - v1.y * v2.x));
}

#endif // vec3_H