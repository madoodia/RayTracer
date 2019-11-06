// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera
{
public:
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect)
	{
		vec3 right, up, w;
		float theta = vfov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = (lookFrom - lookAt).normalize();
		right = cross(vup, w).normalize();
		up = cross(w, right);
		//llc = vec3(-halfWidth, -halfHeight, -1.0);
		llc = origin - halfWidth * right - halfHeight * up - w;
		horizontal = 2 * halfWidth * right;
		vertical = 2 * halfHeight * up;
	}
	Ray getRay(float u, float v) { return Ray(origin, llc + u * horizontal + v * vertical - origin); }

private:
	vec3 llc; // lower left corner
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
};

#endif // CAMERA_H
