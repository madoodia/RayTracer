// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "hittable.h"

class Translate : public Hittable
{
public:
    Translate(Hittable *p, const vec3 &displacement)
        : ptr(p), offset(displacement) {}
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const;
    Hittable *ptr;
    vec3 offset;
};

bool Translate::hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const
{
    Ray movedR(ray.origin() - offset, ray.direction(), ray.time());
    if (ptr->hit(movedR, tMin, tMax, rec))
    {
        rec.p += offset;
        return true;
    }
    else
        return false;
}

bool Translate::boundingBox(float t0, float t1, AABB &box) const
{
    if (ptr->boundingBox(t0, t1, box))
    {
        box = AABB(box.min() + offset, box.max() + offset);
        return true;
    }
    else
        return false;
}

class RotateY : public Hittable
{
public:
    RotateY(Hittable *p, float angle);
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = bbox;
        return hasbox;
    }
    Hittable *ptr;
    float sinTheta;
    float cosTheta;
    bool hasbox;
    AABB bbox;
};

RotateY::RotateY(Hittable *p, float angle) : ptr(p)
{
    float radians = (M_PI / 180.) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newx = cosTheta * x + sinTheta * z;
                float newz = -sinTheta * x + cosTheta * z;
                vec3 tester(newx, y, newz);
                for (int c = 0; c < 3; c++)
                {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool RotateY::hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const
{
    vec3 origin = ray.origin();
    vec3 direction = ray.direction();
    origin[0] = cosTheta * ray.origin()[0] - sinTheta * ray.origin()[2];
    origin[2] = sinTheta * ray.origin()[0] + cosTheta * ray.origin()[2];
    direction[0] = cosTheta * ray.direction()[0] - sinTheta * ray.direction()[2];
    direction[2] = sinTheta * ray.direction()[0] + cosTheta * ray.direction()[2];
    Ray RotatedR(origin, direction, ray.time());
    if (ptr->hit(RotatedR, tMin, tMax, rec))
    {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
        p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];
        normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
        normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

class RotateX : public Hittable
{
public:
    RotateX(Hittable *p, float angle);
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = bbox;
        return hasbox;
    }
    Hittable *ptr;
    float sinTheta;
    float cosTheta;
    bool hasbox;
    AABB bbox;
};

RotateX::RotateX(Hittable *p, float angle) : ptr(p)
{
    float radians = (M_PI / 180.) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newy = cosTheta * y - sinTheta * z;
                float newz = sinTheta * y + cosTheta * z;
                vec3 tester(x, newy, newz);
                for (int c = 0; c < 3; c++)
                {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool RotateX::hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const
{
    vec3 origin = ray.origin();
    vec3 direction = ray.direction();
    origin[1] = cosTheta * ray.origin()[1] - sinTheta * ray.origin()[2];
    origin[2] = sinTheta * ray.origin()[1] + cosTheta * ray.origin()[2];
    direction[1] = cosTheta * ray.direction()[1] - sinTheta * ray.direction()[2];
    direction[2] = sinTheta * ray.direction()[1] + cosTheta * ray.direction()[2];
    Ray RotatedR(origin, direction, ray.time());
    if (ptr->hit(RotatedR, tMin, tMax, rec))
    {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        p[1] = cosTheta * rec.p[1] + sinTheta * rec.p[2];
        p[2] = -sinTheta * rec.p[1] + cosTheta * rec.p[2];
        normal[1] = cosTheta * rec.normal[1] + sinTheta * rec.normal[2];
        normal[2] = -sinTheta * rec.normal[1] + cosTheta * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

class RotateZ : public Hittable
{
public:
    RotateZ(Hittable *p, float angle);
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        box = bbox;
        return hasbox;
    }
    Hittable *ptr;
    float sinTheta;
    float cosTheta;
    bool hasbox;
    AABB bbox;
};

RotateZ::RotateZ(Hittable *p, float angle) : ptr(p)
{
    float radians = (M_PI / 180.) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newx = cosTheta * x - sinTheta * y;
                float newy = sinTheta * x + cosTheta * y;
                vec3 tester(newx, newy, z);
                for (int c = 0; c < 3; c++)
                {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = AABB(min, max);
}

bool RotateZ::hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const
{
    vec3 origin = ray.origin();
    vec3 direction = ray.direction();
    origin[0] = cosTheta * ray.origin()[0] - sinTheta * ray.origin()[1];
    origin[1] = sinTheta * ray.origin()[0] + cosTheta * ray.origin()[1];
    direction[0] = cosTheta * ray.direction()[0] - sinTheta * ray.direction()[1];
    direction[1] = sinTheta * ray.direction()[0] + cosTheta * ray.direction()[1];
    Ray RotatedR(origin, direction, ray.time());
    if (ptr->hit(RotatedR, tMin, tMax, rec))
    {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[1];
        p[1] = -sinTheta * rec.p[0] + cosTheta * rec.p[1];
        normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[1];
        normal[1] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[1];
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

#endif // TRANSLATION_H
