// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"

class HittableList : public Hittable
{
public:
	Hittable **list;
	int listSize;

public:
	HittableList() {}
	HittableList(Hittable **l, int n) : list(l), listSize(n) {}
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const;
	virtual bool boundingBox(float t0, float t1, AABB &box) const;
	virtual float pdfValue(const vec3 &o, const vec3 &v) const;
	virtual vec3 random(const vec3 &o) const;
};

float HittableList::pdfValue(const vec3 &o, const vec3 &v) const
{
	float weight = 1.0 / listSize;
	float sum = 0;
	for (int i = 0; i < listSize; i++)
		sum += weight * list[i]->pdfValue(o, v);
	return sum;
}

vec3 HittableList::random(const vec3 &o) const
{
	int index = int(randomDouble() * listSize);
	return list[index]->random(o);
}

bool HittableList::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
	HitRecord tempRecord;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->hit(ray, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}

bool HittableList::boundingBox(float t0, float t1, AABB &box) const
{
	if (listSize < 1)
		return false;
	AABB tempBox;
	bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue)
		return false;
	else
		box = tempBox;
	for (int i = 1; i < listSize; i++)
	{
		if (list[i]->boundingBox(t0, t1, tempBox))
		{
			box = surroundingBox(box, tempBox);
		}
		else
			return false;
	}
	return true;
}

#endif // HITTABLELIST_H
