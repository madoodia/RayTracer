// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "common.h"
#include "aabb.h"

class Hitable
{
public:
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;
	virtual bool boundingBox(float t0, float t1, AABB &box) const = 0;
};

#endif // HITABLE_H
