// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef VOLUME_H
#define VOLUME_H

#include "hittable.h"
#include "material.h"

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(Hittable *b, float d, Texture *a) : boundary(b), density(d)
    {
        phaseFunction = new Isotropic(a);
    }
    virtual bool hit(
        const Ray &rayr, float tMin, float tMax, HitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const
    {
        return boundary->boundingBox(t0, t1, box);
    }
    Hittable *boundary;
    float density;
    Material *phaseFunction;
};

bool ConstantMedium::hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const
{
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    bool debugging = enableDebug && randomDouble() < 0.00001;

    HitRecord rec1, rec2;

    if (boundary->hit(ray, -FLT_MAX, FLT_MAX, rec1))
    {
        if (boundary->hit(ray, rec1.t + 0.0001, FLT_MAX, rec2))
        {

            if (debugging)
                std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << '\n';

            if (rec1.t < tMin)
                rec1.t = tMin;

            if (rec2.t > tMax)
                rec2.t = tMax;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            float distanceInsideBoundary = (rec2.t - rec1.t) * ray.direction().length();
            float hitDistance = -(1 / density) * log(randomDouble());

            if (hitDistance < distanceInsideBoundary)
            {

                rec.t = rec1.t + hitDistance / ray.direction().length();
                rec.p = ray.pFunction(rec.t);

                if (debugging)
                {
                    std::cerr << "hitDistance = " << hitDistance << '\n'
                              << "rec.t = " << rec.t << '\n'
                              << "rec.p = " << rec.p << '\n';
                }

                rec.normal = vec3(1, 0, 0); // arbitrary
                rec.matPtr = phaseFunction;
                return true;
            }
        }
    }
    return false;
}

#endif //VOLUME_H
