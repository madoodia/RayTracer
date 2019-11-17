// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef PDF_H
#define PDF_H

#include "vec3.h"
#include "onb.h"
#include "common.h"
#include "hittable.h"

class PDF
{
public:
    virtual float value(const vec3 &direction) const = 0;
    virtual vec3 generate() const = 0;
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const vec3 &w) { uvw.buildFromW(w); }
    virtual float value(const vec3 &direction) const
    {
        float cosine = dot(direction.normalize(), uvw.w());
        if (cosine > 0)
            return cosine / M_PI;
        else
            return 0;
    }
    virtual vec3 generate() const
    {
        return uvw.local(randomCosineDirection());
    }
    ONB uvw;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(Hittable *p, const vec3 &origin) : ptr(p), o(origin) {}
    virtual float value(const vec3 &direction) const
    {
        return ptr->pdfValue(o, direction);
    }
    virtual vec3 generate() const
    {
        return ptr->random(o);
    }
    vec3 o;
    Hittable *ptr;
};

class MixturePDF : public PDF
{
public:
    MixturePDF(PDF *p0, PDF *p1)
    {
        p[0] = p0;
        p[1] = p1;
    }
    virtual float value(const vec3 &direction) const
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }
    virtual vec3 generate() const
    {
        if (randomDouble() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }
    PDF *p[2];
};

#endif PDF_H