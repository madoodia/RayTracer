// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

class Dielectric : public Material
{
public:
	float refIdx;

public:
	Dielectric(float ri) : refIdx(ri) {}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &attenuation, Ray &scattered) const
	{
		vec3 outwardNormal;
		vec3 reflected = reflect(rayIn.direction(), record.normal);
		float niOnt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflectProb;
		float cosine;
		if (dot(rayIn.direction(), record.normal) > 0)
		{
			outwardNormal = -record.normal;
			niOnt = refIdx;
			//cosine = refIdx * dot(rayIn.direction(), record.normal) / rayIn.direction().length();
			cosine = dot(rayIn.direction(), record.normal) / rayIn.direction().length();
			cosine = sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
		}
		else
		{
			outwardNormal = record.normal;
			niOnt = 1.0 / refIdx;
			cosine = -dot(rayIn.direction(), record.normal) / rayIn.direction().length();
		}
		if (refract(rayIn.direction(), outwardNormal, niOnt, refracted))
		{
			reflectProb = schlick(cosine, refIdx);
		}
		else
		{
			reflectProb = 1.0;
		}
		if (randomDouble() < reflectProb)
		{
			scattered = Ray(record.p, reflected, rayIn.time());
		}
		else
		{
			scattered = Ray(record.p, refracted, rayIn.time());
		}
		return true;
	}
};

#endif // DIELECTRIC_H
