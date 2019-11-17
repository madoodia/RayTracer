// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "hittable.h"

class Material;

class XYRect : public Hittable
{
public:
    XYRect() {}
    XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material *mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};
    virtual bool hit(const Ray &ray, float t0, float t1, HitRecord &record) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = AABB(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
        return true;
    }
    Material *mp;
    float x0, x1, y0, y1, k;
};

bool XYRect::hit(const Ray &ray, float t0, float t1, HitRecord &record) const
{
    float t = (k - ray.origin().z) / ray.direction().z;
    if (t < t0 || t > t1)
        return false;
    float x = ray.origin().x + t * ray.direction().x;
    float y = ray.origin().y + t * ray.direction().y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    record.u = (x - x0) / (x1 - x0);
    record.v = (y - y0) / (y1 - y0);
    record.t = t;
    record.matPtr = mp;
    record.p = ray.pFunction(t);
    record.normal = vec3(0, 0, 1);
    return true;
}

class XZRect : public Hittable
{
public:
    XZRect() {}
    XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material *mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};
    virtual bool hit(const Ray &ray, float t0, float t1, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = AABB(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
        return true;
    }

    virtual float pdfValue(const vec3 &o, const vec3 &v) const
    {
        HitRecord rec;
        if (this->hit(Ray(o, v), 0.001, FLT_MAX, rec))
        {
            float area = (x1 - x0) * (z1 - z0);
            float distanceSquared = rec.t * rec.t * v.squareLength();
            float cosine = fabs(dot(v, rec.normal) / v.length());
            return distanceSquared / (cosine * area);
        }
        else
            return 0;
    }
    virtual vec3 random(const vec3 &o) const
    {
        vec3 randomPoint = vec3(x0 + randomDouble() * (x1 - x0), k,
                                z0 + randomDouble() * (z1 - z0));
        return randomPoint - o;
    }

    Material *mp;
    float x0, x1, z0, z1, k;
};

bool XZRect::hit(const Ray &ray, float t0, float t1, HitRecord &rec) const
{
    float t = (k - ray.origin().y) / ray.direction().y;
    if (t < t0 || t > t1)
        return false;
    float x = ray.origin().x + t * ray.direction().x;
    float z = ray.origin().z + t * ray.direction().z;
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = ray.pFunction(t);
    rec.normal = vec3(0, 1, 0);
    return true;
}

class YZRect : public Hittable
{
public:
    YZRect() {}
    YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material *mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};
    virtual bool hit(const Ray &ray, float t0, float t1, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = AABB(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
        return true;
    }
    Material *mp;
    float y0, y1, z0, z1, k;
};

bool YZRect::hit(const Ray &ray, float t0, float t1, HitRecord &rec) const
{
    float t = (k - ray.origin().x) / ray.direction().x;
    if (t < t0 || t > t1)
        return false;
    float y = ray.origin().y + t * ray.direction().y;
    float z = ray.origin().z + t * ray.direction().z;
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = ray.pFunction(t);
    rec.normal = vec3(1, 0, 0);
    return true;
}

#endif // RECTANGLE_H