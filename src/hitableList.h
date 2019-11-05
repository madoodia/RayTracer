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
	Hitable** list;
	int listSize;
public:
	HitableList() :list(NULL), listSize(0) {}
	HitableList(Hitable** l, int n) : list(l), listSize(n) {}
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
};

bool HitableList::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	HitRecord tempRecord;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for(int i = 0; i < listSize; i++)
	{
		if(list[i]->hit(ray, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}

#endif // HITABLELIST_H
