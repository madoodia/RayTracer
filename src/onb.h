// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef ONB_H
#define ONB_H

#include "vec3.h"

class ONB
{
public:
    ONB() {}
    inline vec3 operator[](int i) const { return axis[i]; }
    vec3 u() const { return axis[0]; }
    vec3 v() const { return axis[1]; }
    vec3 w() const { return axis[2]; }
    vec3 local(float a, float b, float c) const
    {
        return a * u() + b * v() + c * w();
    }
    vec3 local(const vec3 &a) const
    {
        return a.x * u() + a.y * v() + a.z * w();
    }
    void buildFromW(const vec3 &);
    vec3 axis[3];
};

void ONB::buildFromW(const vec3 &n)
{
    axis[2] = n.normalize();
    vec3 a;
    if (fabs(w().x) > 0.9)
        a = vec3(0, 1, 0);
    else
        a = vec3(1, 0, 0);
    axis[1] = cross(w(), a).normalize();
    axis[0] = cross(w(), v());
}

#endif ONB_H