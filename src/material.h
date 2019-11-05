// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"

class Material
{
public:
	Material() {}
	virtual bool scatter(const Ray& rayIn, const HitRecord& record, vec3& attenuation, Ray& scattered) const = 0;
};

#endif // MATERIAL_H
