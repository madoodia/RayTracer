// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "common.h"

class Camera
{
public:
	vec3 llc; // lower left corner
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
	float lensRadius;
	vec3 right, up, w;

public:
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vup,
		   float vfov, float aspect, float aperture, float dof)
	{
		lensRadius = aperture / 2;
		float theta = vfov * M_PI / 180; // change degree to radian
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = (lookFrom - lookAt).normalize();
		right = cross(vup, w).normalize();
		up = cross(w, right);
		//llc = vec3(-halfWidth, -halfHeight, -1.0);
		llc = origin - halfWidth * dof * right - halfHeight * dof * up - dof * w;
		horizontal = 2 * halfWidth * dof * right;
		vertical = 2 * halfHeight * dof * up;
	}
	Ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * randomOnDisk();
		vec3 offset = right * rd.x + up * rd.y;
		return Ray(origin + offset, llc + s * horizontal + t * vertical - origin - offset);
	}
};

#endif // CAMERA_H
