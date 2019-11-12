//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"

class Texture
{
public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class ConstantTexture : public Texture
{
public:
    ConstantTexture() {}
    ConstantTexture(vec3 c) : color(c) {}
    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        return color;
    }
    vec3 color;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture() {}
    CheckerTexture(Texture *t0, Texture *t1) : even(t0), odd(t1) {}
    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    Texture *odd;
    Texture *even;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture() {}
    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        return vec3(1, 1, 1) * noise.noise(p);
    }
    Perlin noise;
};

#endif // TEXTURE_H