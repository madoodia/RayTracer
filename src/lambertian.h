// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"

class Lambertian : public Material
{
public:
	vec3 albedo;

public:
	Lambertian(const vec3 &a) : albedo(a) {}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &attenuation, Ray &scattered) const
	{
		vec3 target = record.p + record.normal + randomOnSphere();
		scattered = Ray(record.p, target - record.p, rayIn.time());
		attenuation = albedo;
		return true;
	}
};

#endif // LAMBERTIAN_H
