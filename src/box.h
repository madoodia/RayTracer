// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef BOX_H
#define BOX_H

#include "hittable.h"
#include "flipNormal.h"
#include "rectangle.h"
#include "hittableList.h"

class Box : public Hittable
{
public:
    Box() {}
    Box(const vec3 &p0, const vec3 &p1, Material *ptr);
    virtual bool hit(const Ray &ray, float t0, float t1, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = AABB(pmin, pmax);
        return true;
    }
    vec3 pmin, pmax;
    Hittable *listPtr;
};

Box::Box(const vec3 &p0, const vec3 &p1, Material *ptr)
{
    pmin = p0;
    pmax = p1;
    Hittable **list = new Hittable *[6];
    list[0] = new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, ptr);
    list[1] = new FlipNormal(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));
    list[2] = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, ptr);
    list[3] = new FlipNormal(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));
    list[4] = new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, ptr);
    list[5] = new FlipNormal(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
    listPtr = new HittableList(list, 6);
}

bool Box::hit(const Ray &ray, float t0, float t1, HitRecord &rec) const
{
    return listPtr->hit(ray, t0, t1, rec);
}

#endif // BOX_H