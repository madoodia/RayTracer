// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "common.h"

class Hitable
{
public:
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const = 0;
};

#endif // HITABLE_H
