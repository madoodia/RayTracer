// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------
#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "hitable.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB
{
public:
    vec3 _min;
    vec3 _max;

public:
    AABB() {}
    AABB(const vec3 &a, const vec3 &b) { _min = a, _max = b; }

    vec3 min() const { return _min; }
    vec3 max() const { return _max; }

    bool hit(const Ray &r, float tmin, float tmax) const
    {
        for (int a = 0; a < 3; a++)
        {
            float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
                             (_max[a] - r.origin()[a]) / r.direction()[a]);
            float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
                             (_max[a] - r.origin()[a]) / r.direction()[a]);

            tmin = ffmin(t0, tmin);
            tmax = ffmax(t1, tmax);
            if (tmax <= tmin)
                return false;
        }
        return true;
    }
};

AABB surroundingBox(AABB box0, AABB box1)
{
    vec3 small(
        ffmin(box0.min().x, box1.min().x),
        ffmin(box0.min().y, box1.min().y),
        ffmin(box0.min().z, box1.min().z));
    vec3 big(
        ffmax(box0.max().x, box1.max().x),
        ffmax(box0.max().y, box1.max().y),
        ffmax(box0.max().z, box1.max().z));

    return AABB(small, big);
}

#endif //AABB_H