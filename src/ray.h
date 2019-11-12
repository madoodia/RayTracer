// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.h"

class Ray
{
public:
	vec3 A;
	vec3 B;
	float _time;

public:
	Ray() {}
	Ray(const vec3 &orig, const vec3 &dir, float ti = 0.0)
	{
		A = orig;
		B = dir;
		_time = ti;
	}
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float time() const { return _time; }
	vec3 pFunction(float t) const { return A + t * B; }
};

#endif // RAY_H
