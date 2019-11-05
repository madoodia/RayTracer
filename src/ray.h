// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const vec3& orig, const vec3& dir) { A = orig; B = dir; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 pFunction(float t) const { return A + t * B; }

private:
	vec3 A;
	vec3 B;
};

#endif // RAY_H
