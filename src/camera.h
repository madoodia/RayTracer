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
	Camera()
	{
		llc = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}
	Ray getRay(float u, float v) { return Ray(origin, llc + u * horizontal + v * vertical - origin); }

private:
	vec3 llc; // lower left corner
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
};

#endif // CAMERA_H
