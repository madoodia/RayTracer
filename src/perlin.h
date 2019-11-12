
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

inline float trilinearInterp(float c[2][2][2], float u, float v, float w)
{
    float accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accum;
}

inline float perlinInterp(vec3 c[2][2][2], float u, float v, float w)
{
    float uu = u * u * (3 - 2 * u);
    float vv = v * v * (3 - 2 * v);
    float ww = w * w * (3 - 2 * w);
    float accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                vec3 weightV(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         dot(c[i][j][k], weightV);
            }
    return accum;
}

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

        vec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[permX[(i + di) & 255] ^
                                           permY[(j + dj) & 255] ^
                                           permZ[(k + dk) & 255]];
        return perlinInterp(c, u, v, w);
    }

    static vec3 *ranvec;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static vec3 *perlinGenerate()
{
    vec3 *p = new vec3[256];
    for (int i = 0; i < 256; ++i)
    {
        float xRand = 2 * randomDouble() - 1;
        float yRand = 2 * randomDouble() - 1;
        float zRand = 2 * randomDouble() - 1;
        p[i] = vec3(xRand, yRand, zRand).normalize();
    }
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

vec3 *Perlin::ranvec = perlinGenerate();
int *Perlin::permX = perlinGeneratePerm();
int *Perlin::permY = perlinGeneratePerm();
int *Perlin::permZ = perlinGeneratePerm();

#endif //PERLIN_H
