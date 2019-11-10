// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "hitable.h"

class HitableList : public Hitable
{
public:
	Hitable **list;
	int listSize;

public:
	HitableList() {}
	HitableList(Hitable **l, int n) : list(l), listSize(n) {}
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const;
	virtual bool boundingBox(float t0, float t1, AABB &box) const;
};

bool HitableList::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
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

bool HitableList::boundingBox(float t0, float t1, AABB &box) const
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

#endif // HITABLELIST_H
