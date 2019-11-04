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

vec3 setColor(const Ray& r)
{
	vec3 unit = r.direction().normalize();
	float t = 0.5f * (unit.y + 1.0f);
	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
}

int main()
{
	// Calculate Time for running the code
	// inline functions and regular functions.
	{
		Timer timer;
		int nx = 500;
		int ny = 300;

		std::ofstream imageFile;
		imageFile.open("../src/image.ppm");

		imageFile << "P3\n" << nx << " " << ny << "\n255\n";

		// camera
		vec3 llc(-2.0f, -1.0f, -1.0f); // lower left corner
		vec3 horizontal(4.0f, 0.0f, 0.0f);
		vec3 vertical(0.0f, 2.0f, 0.0f);
		vec3 origin(0.0f, 0.0f, 0.0f);

		for(int j = ny - 1; j >= 0; j--)
		{
			for(int i = 0; i < nx; i++)
			{
				float u = float(i) / float(nx);
				float v = float(j) / float(ny);

				Ray r(origin, llc + u * horizontal + v * vertical);
				vec3 col = setColor(r);
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