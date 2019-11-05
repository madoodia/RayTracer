// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

// PPM Viewer: http://web.eecs.utk.edu/~smarz1/pgmview/

// C++ Headers

// Third Party Headers

// Own Headers

#include <iostream>
#include <fstream>
#include <float.h>

#include "myTimer.h"
#include "sphere.h"
#include "hitableList.h"
#include "camera.h"

vec3 setColor(const Ray& ray, Hitable* world)
{
	HitRecord record;
	if(world->hit(ray, 0.0, 1000, record))
	{
		return 0.5 * vec3(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1);
	}
	else
	{
		vec3 unit = ray.direction().normalize();
		float t = 0.5 * (unit.y + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	// Calculate Time for running the code
	// inline functions and regular functions.
	{
		Timer timer;
		int nx = 500;
		int ny = 250;

		std::ofstream imageFile;
		imageFile.open("../output/outputImage.ppm");

		imageFile << "P3\n" << nx << " " << ny << "\n255\n";

		// camera
		Camera cam;

		Hitable* list[2];
		list[0] = new Sphere(vec3(0.0, 0.0, -1.0), 0.5);
		list[1] = new Sphere(vec3(0.0, -100.5, -1.0), 100);

		Hitable* world = new HitableList(list, 2);

		for(int j = ny - 1; j >= 0; j--)
		{
			for(int i = 0; i < nx; i++)
			{
				float u = float(i) / float(nx);
				float v = float(j) / float(ny);

				Ray ray = cam.getRay(u, v);
				vec3 p = ray.pFunction(2.0);
				vec3 col = setColor(ray, world);
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