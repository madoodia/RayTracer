// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "common.h"
#include "aabb.h"

class Hittable
{
public:
	virtual bool hit(const Ray &r, float tMin, float tMax,
					 HitRecord &rec) const = 0;
	virtual bool boundingBox(float t0, float t1, AABB &box) const = 0;
	virtual float pdfValue(const vec3 &o, const vec3 &v) const { return 0.0; }
	virtual vec3 random(const vec3 &o) const { return vec3(1, 0, 0); }
};

#endif // HITTABLE_H
