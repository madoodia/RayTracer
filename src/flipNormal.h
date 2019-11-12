// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef FLIPNORMAL_H
#define FLIPNORMAL_H

#include "hittable.h"

class FlipNormal : public Hittable
{
public:
    FlipNormal(Hittable *p) : ptr(p) {}
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
    {

        if (ptr->hit(ray, tMin, tMax, record))
        {
            record.normal = -record.normal;
            return true;
        }
        else
            return false;
    }

    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        return ptr->boundingBox(t0, t1, box);
    }

    Hittable *ptr;
};

#endif // FLIPNORMAL_H