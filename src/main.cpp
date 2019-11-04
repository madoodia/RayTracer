// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

// C++ Headers

// Third Party Headers

// Own Headers

#include <iostream>
#include <fstream>

#include "vec3.h"
#include "myTimer.h"
#include "ray.h"

bool hitSphere(const vec3& center, float radius, const Ray& ray)
{
	vec3 oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = 2.0 * dot(oc, ray.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return discriminant > 0.0;
}

vec3 setColor(const Ray& ray)
{
	if(hitSphere(vec3(0, 0, -1), 0.5, ray))
	{
		return vec3(1, 0, 0);
	}
	vec3 unit = ray.direction().normalize();
	float t = 0.5 * (unit.y + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	// Calculate Time for running the code
	// inline functions and regular functions.
	{
		Timer timer;
		int nx = 400;
		int ny = 200;

		std::ofstream imageFile;
		imageFile.open("../src/image.ppm");

		imageFile << "P3\n" << nx << " " << ny << "\n255\n";

		// camera
		vec3 llc(-2.0, -1.0, -1.0); // lower left corner
		vec3 horizontal(4.0, 0.0, 0.0);
		vec3 vertical(0.0, 2.0, 0.0);
		vec3 origin(0.0, 0.0, 0.0);

		for(int j = ny - 1; j >= 0; j--)
		{
			for(int i = 0; i < nx; i++)
			{
				float u = float(i) / float(nx);
				float v = float(j) / float(ny);

				Ray ray(origin, llc + u * horizontal + v * vertical);
				vec3 col = setColor(ray);
				int ir = int(255.99 * col.x);
				int ig = int(255.99 * col.y);
				int ib = int(255.99 * col.z);
				//std::cout << ir << " " << ig << " " << ib << "\n";
				imageFile << ir << " " << ig << " " << ib << "\n";
			}
		}
		imageFile.close();
		std::cout << "Time is: ";
	}
	return 0;
}