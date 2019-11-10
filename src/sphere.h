// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class Material;

class Sphere : public Hitable
{
public:
	vec3 center;
	float radius;
	Material *material;

public:
	Sphere() {}
	Sphere(vec3 cen, float r, Material *mat)
		: center(cen), radius(r), material(mat) {}
	virtual bool hit(const Ray &ray, float tMin, float tMax,
					 HitRecord &record) const;
	virtual bool boundingBox(float t0, float t1, AABB &box) const;
};

bool Sphere::boundingBox(float t0, float t1, AABB &box) const
{
	box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax,
				 HitRecord &record) const
{
	vec3 oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius * radius;
	float discreminant = b * b - a * c;
	if (discreminant > 0.0)
	{
		float temp = (-b - sqrt(discreminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pFunction(record.t);
			record.normal = (record.p - center) / radius;
			record.matPtr = material;
			return true;
		}
		temp = (-b + sqrt(discreminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pFunction(record.t);
			record.normal = (record.p - center) / radius;
			record.matPtr = material;
			return true;
		}
	}
	return false;
}

class MovingSphere : public Hitable
{
public:
	float time0, time1;
	vec3 center0, center1;
	float radius;
	Material *matPtr;

public:
	MovingSphere() {}
	MovingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, Material *m)
		: center0(cen0),
		  center1(cen1),
		  time0(t0),
		  time1(t1),
		  radius(r),
		  matPtr(m) {}
	virtual bool hit(const Ray &ray, float tMin, float tMax,
					 HitRecord &record) const;
	virtual bool boundingBox(float t0, float t1, AABB &box) const;
	vec3 center(float time) const;
};

vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::boundingBox(float t0, float t1, AABB &box) const
{
	AABB box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	AABB box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surroundingBox(box0, box1);
	return true;
}

bool MovingSphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const
{
	vec3 oc = ray.origin() - center(ray.time());
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius * radius;
	float discreminant = b * b - a * c;
	if (discreminant > 0)
	{
		float temp = (-b - sqrt(discreminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pFunction(record.t);
			record.normal = (record.p - center(ray.time())) / radius;
			record.matPtr = matPtr;
			return true;
		}
		temp = (-b + sqrt(discreminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pFunction(record.t);
			record.normal = (record.p - center(ray.time())) / radius;
			record.matPtr = matPtr;
			return true;
		}
	}
	return false;
}
#endif // SPHERE_H
