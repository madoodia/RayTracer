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

#ifndef BVHH
#define BVHH

#include "hittable.h"

class bvhNode : public Hittable
{
public:
    bvhNode() {}
    bvhNode(Hittable **l, int n, float time0, float time1);
    virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const;
    virtual bool boundingBox(float t0, float t1, AABB &box) const;
    Hittable *left;
    Hittable *right;
    AABB box;
};

bool bvhNode::boundingBox(float t0, float t1, AABB &b) const
{
    b = box;
    return true;
}

bool bvhNode::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
    if (box.hit(ray, tMin, tMax))
    {
        HitRecord leftRec, rightRec;
        bool hitLeft = left->hit(ray, tMin, tMax, leftRec);
        bool hitRight = right->hit(ray, tMin, tMax, rightRec);
        if (hitLeft && hitRight)
        {
            if (leftRec.t < rightRec.t)
                record = leftRec;
            else
                record = rightRec;
            return true;
        }
        else if (hitLeft)
        {
            record = leftRec;
            return true;
        }
        else if (hitRight)
        {
            record = rightRec;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

int boxXCompare(const void *a, const void *b)
{
    AABB boxLeft, boxRight;
    Hittable *ah = *(Hittable **)a;
    Hittable *bh = *(Hittable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor\n";
    if (boxLeft.min().x - boxRight.min().x < 0.0)
        return -1;
    else
        return 1;
}

int boxYCompare(const void *a, const void *b)
{
    AABB boxLeft, boxRight;
    Hittable *ah = *(Hittable **)a;
    Hittable *bh = *(Hittable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor\n";
    if (boxLeft.min().y - boxRight.min().y < 0.0)
        return -1;
    else
        return 1;
}
int boxZCompare(const void *a, const void *b)
{
    AABB boxLeft, boxRight;
    Hittable *ah = *(Hittable **)a;
    Hittable *bh = *(Hittable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor\n";
    if (boxLeft.min().z - boxRight.min().z < 0.0)
        return -1;
    else
        return 1;
}

bvhNode::bvhNode(Hittable **l, int n, float time0, float time1)
{
    int axis = int(3 * randomDouble());
    if (axis == 0)
        qsort(l, n, sizeof(Hittable *), boxXCompare);
    else if (axis == 1)
        qsort(l, n, sizeof(Hittable *), boxYCompare);
    else
        qsort(l, n, sizeof(Hittable *), boxZCompare);
    if (n == 1)
    {
        left = right = l[0];
    }
    else if (n == 2)
    {
        left = l[0];
        right = l[1];
    }
    else
    {
        left = new bvhNode(l, n / 2, time0, time1);
        right = new bvhNode(l + n / 2, n - n / 2, time0, time1);
    }
    AABB boxLeft, boxRight;
    if (!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))
        std::cerr << "no bounding box in bvhNode constructor\n";
    box = surroundingBox(boxLeft, boxRight);
}

#endif
