// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "onb.h"

class Material
{
public:
	Material() {}
	virtual bool scatter(const Ray &rayIn, const HitRecord &record, vec3 &albedo,
						 Ray &scattered, float &pdf) const
	{
		return false;
	}
	virtual float scatteringPDF(const Ray &rayIn, const HitRecord &record,
								const Ray &scattered) const
	{
		return 0;
	}
	virtual vec3 emitted(float u, float v, const vec3 &p) const
	{
		return vec3(0, 0, 0);
	}
};

class Lambertian : public Material
{
public:
	Texture *albedo;

public:
	Lambertian(Texture *a) : albedo(a) {}
	bool scatter(const Ray &rayIn,
				 const HitRecord &record,
				 vec3 &alb,
				 Ray &scattered,
				 float &pdf) const
	{
		ONB uvw;
		uvw.buildFromW(record.normal);
		vec3 direction = uvw.local(randomCosineDirection());
		scattered = Ray(record.p, direction.normalize(), rayIn.time());
		alb = albedo->value(record.u, record.v, record.p);
		pdf = dot(uvw.w(), scattered.direction()) / M_PI;
		return true;
	}

	float scatteringPDF(const Ray &rayIn, const HitRecord &record, const Ray &scattered) const
	{
		float cosine = dot(record.normal, scattered.direction().normalize());
		if (cosine < 0)
			return 0;
		return cosine / M_PI;
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
			cosine = refIdx * dot(rayIn.direction(), record.normal) / rayIn.direction().length();
			// cosine = dot(rayIn.direction(), record.normal) / rayIn.direction().length();
			// cosine = sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
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
			scattered = Ray(record.p, reflected, rayIn.time());
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
	virtual bool scatter(const Ray &rayIn, const HitRecord &rec,
						 vec3 &attenuation, Ray &scattered) const { return false; }
	virtual vec3 emitted(float u, float v, const vec3 &p) const
	{
		return emit->value(u, v, p);
	}
	Texture *emit;
};

class Isotropic : public Material
{
public:
	Isotropic(Texture *a) : albedo(a) {}
	virtual bool scatter(
		const Ray &rayIn,
		const HitRecord &rec,
		vec3 &attenuation,
		Ray &scattered) const
	{
		scattered = Ray(rec.p, randomOnSphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	Texture *albedo;
};

#endif // MATERIAL_H
