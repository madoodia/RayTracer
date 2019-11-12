// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "texture.h"

class Lambertian : public Material
{
public:
	Texture *albedo;

public:
	Lambertian(Texture *a) : albedo(a) {}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &attenuation, Ray &scattered) const
	{
		vec3 target = record.p + record.normal + randomOnSphere();
		scattered = Ray(record.p, target - record.p, rayIn.time());
		attenuation = albedo->value(0, 0, record.p);
		return true;
	}
};

#endif // LAMBERTIAN_H
