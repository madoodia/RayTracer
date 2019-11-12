
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

#ifndef PERLIN_H
#define PERLIN_H

#include "vec3.h"

class Perlin
{
public:
    float noise(const vec3 &p) const
    {
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);
        int i = floor(p.x);
        int j = floor(p.y);
        int k = floor(p.z);
        return ranfloat[(permX[i] & 255) ^ (permY[j] & 255) ^ (permZ[k] & 255)];
    }

    static float *ranfloat;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static float *perlinGenerate()
{
    float *p = new float[256];
    for (int i = 0; i < 256; ++i)
        p[i] = randomDouble();
    return p;
}

void permute(int *p, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = int(randomDouble() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int *perlinGeneratePerm()
{
    int *p = new int[256];
    for (int i = 0; i < 256; i++)
        p[i] = i;
    permute(p, 256);
    return p;
}

float *Perlin::ranfloat = perlinGenerate();
int *Perlin::permX = perlinGeneratePerm();
int *Perlin::permY = perlinGeneratePerm();
int *Perlin::permZ = perlinGeneratePerm();

#endif //PERLIN_H
