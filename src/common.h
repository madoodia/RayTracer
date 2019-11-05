// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef COMMON_H
#define COMMON_H

#include "vec3.h"

class Material;

struct HitRecord
{
	float t;
	vec3 p;
	vec3 normal;
	Material* matPtr;
};

float createRandom()
{
	//This will generate a number from 0.0 to 1.0, inclusive.

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//This will generate a number from 0.0 to some arbitrary float, X:

	//float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
	//This will generate a number from some arbitrary LO to some arbitrary HI:

	//float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r;
}

vec3 randomOnSphere()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(createRandom(), createRandom(), createRandom()) - vec3(1, 1, 1);
	} while(dot(p, p) >= 1.0);
	return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

#endif // COMMON_H
