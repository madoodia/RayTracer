// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

struct ScatterRecord
{
	Ray specularRay;
	bool isSpecular;
	vec3 attenuation;
	PDF *pdfPtr;
};

class Material
{
public:
	Material() {}
	virtual bool scatter(const Ray &rayIn,
						 const HitRecord &hrec,
						 ScatterRecord &srec) const
	{
		return false;
	}
	virtual float scatteringPDF(const Ray &rayIn,
								const HitRecord &record,
								const Ray &scattered) const
	{
		return 0;
	}
	virtual vec3 emitted(const Ray &rayIn,
						 const HitRecord &record,
						 float u,
						 float v,
						 const vec3 &p) const
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
				 const HitRecord &hrec,
				 ScatterRecord &srec) const
	{
		srec.isSpecular = false;
		srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
		srec.pdfPtr = new CosinePDF(hrec.normal);
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
	virtual bool scatter(const Ray &rayIn,
						 const HitRecord &hrec,
						 ScatterRecord &srec) const
	{
		vec3 reflected = reflect(rayIn.direction().normalize(), hrec.normal);
		srec.specularRay = Ray(hrec.p, reflected + fuzz * randomOnSphere());
		srec.attenuation = albedo;
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		return true;
	}
};

class Dielectric : public Material
{
public:
	float refIdx;

public:
	Dielectric(float ri) : refIdx(ri) {}
	virtual bool scatter(const Ray &rayIn,
						 const HitRecord &hrec,
						 ScatterRecord &srec) const
	{
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.attenuation = vec3(1.0, 1.0, 1.0);
		vec3 outwardNormal;
		vec3 reflected = reflect(rayIn.direction(), hrec.normal);
		vec3 refracted;
		float niOnt;
		float reflectProb;
		float cosine;
		if (dot(rayIn.direction(), hrec.normal) > 0)
		{
			outwardNormal = -hrec.normal;
			niOnt = refIdx;
			cosine = refIdx * dot(rayIn.direction(), hrec.normal) / rayIn.direction().length();
		}
		else
		{
			outwardNormal = hrec.normal;
			niOnt = 1.0 / refIdx;
			cosine = -dot(rayIn.direction(), hrec.normal) / rayIn.direction().length();
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
			srec.specularRay = Ray(hrec.p, reflected);
		}
		else
		{
			srec.specularRay = Ray(hrec.p, refracted);
		}
		return true;
	}
};

class DiffuseLight : public Material
{
public:
	DiffuseLight(Texture *a) : emit(a) {}
	virtual vec3 emitted(const Ray &rayIn,
						 const HitRecord &rec,
						 float u,
						 float v,
						 const vec3 &p) const
	{
		if (dot(rec.normal, rayIn.direction()) < 0.0)
			return emit->value(u, v, p);
		else
			return vec3(0, 0, 0);
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
