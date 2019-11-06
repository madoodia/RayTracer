// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef COMMON_H
#define COMMON_H

#include "vec3.h"

#define M_PI 3.14159265359

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

bool refract(const vec3& v, const vec3& n, float niOnt, vec3& refracted)
{
	vec3 uv = v.normalize();
	float dt = dot(uv, n);
	float discreminant = 1.0 - niOnt * niOnt * (1 - dt * dt);
	if(discreminant > 0)
	{
		refracted = niOnt * (v - n * dt) - n * sqrt(discreminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float refIdx)
{
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif // COMMON_H
