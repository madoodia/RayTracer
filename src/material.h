// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class Material
{
public:
	Material() {}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &attenuation, Ray &scattered) const = 0;
	virtual vec3 emitted(float u, float v, const vec3 &p) const { return vec3(0, 0, 0); }
};

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
		attenuation = albedo->value(record.u, record.v, record.p);
		return true;
	}
};

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

class DiffuseLight : public Material
{
public:
	DiffuseLight(Texture *a) : emit(a) {}
	virtual bool scatter(const Ray &r_in, const HitRecord &rec,
						 vec3 &attenuation, Ray &scattered) const { return false; }
	virtual vec3 emitted(float u, float v, const vec3 &p) const
	{
		return emit->value(u, v, p);
	}
	Texture *emit;
};

#endif // MATERIAL_H
