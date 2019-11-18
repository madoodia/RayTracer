// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "onb.h"

class Material;

class Sphere : public Hittable
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
	virtual float pdfValue(const vec3 &o, const vec3 &v) const;
	virtual vec3 random(const vec3 &o) const;
};

float Sphere::pdfValue(const vec3 &o, const vec3 &v) const
{
	HitRecord rec;
	if (this->hit(Ray(o, v), 0.001, FLT_MAX, rec))
	{
		float cos_theta_max = sqrt(1 - radius * radius / (center - o).squareLength());
		float solid_angle = 2 * M_PI * (1 - cos_theta_max);
		return 1 / solid_angle;
	}
	else
		return 0;
}

vec3 Sphere::random(const vec3 &o) const
{
	vec3 direction = center - o;
	float distanceSquared = direction.squareLength();
	ONB uvw;
	uvw.buildFromW(direction);
	return uvw.local(randomToSphere(radius, distanceSquared));
}

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
			getSphereUV((record.p - center) / radius, record.u, record.v);
			record.normal = (record.p - center) / radius;
			record.matPtr = material;
			return true;
		}
		temp = (-b + sqrt(discreminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.p = ray.pFunction(record.t);
			getSphereUV((record.p - center) / radius, record.u, record.v);
			record.normal = (record.p - center) / radius;
			record.matPtr = material;
			return true;
		}
	}
	return false;
}

class MovingSphere : public Hittable
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
