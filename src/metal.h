// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef METAL_H
#define METAL_H

#include "material.h"

class Metal : public Material
{
public:
	vec3 albedo;
	float fuzz;

public:
	Metal(const vec3 &a, float f) : albedo(a)
	{
		if (f < 1)
			fuzz = f;
		else
			fuzz = 1;
	}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &attenuation, Ray &scattered) const
	{
		vec3 reflected = reflect(rayIn.direction().normalize(), record.normal);
		scattered = Ray(record.p, reflected + fuzz * randomOnSphere(), rayIn.time());
		attenuation = albedo;
		return (dot(scattered.direction(), record.normal) > 0);
	}
};

#endif // METAL_H
