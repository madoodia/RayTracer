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

#include "common.h"
#include "myTimer.h"
#include "sphere.h"
#include "hitableList.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"


vec3 setColor(const Ray& ray, Hitable* world, int depth)
{
	HitRecord record;
	if(world->hit(ray, 0.0, FLT_MAX, record))
	{
		Ray scattered;
		vec3 attenuation;
		if(depth < 50 && record.matPtr->scatter(ray, record, attenuation, scattered))
		{
			return attenuation * setColor(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
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
		int ns = 200;

		std::ofstream imageFile;
		imageFile.open("../output/outputImage.ppm");

		imageFile << "P3\n" << nx << " " << ny << "\n255\n";

		float R = cos(M_PI / 4);

		Hitable* list[4];
		list[0] = new Sphere(vec3(R, 0.0, -1.0), R, new Lambertian(vec3(0, 0, 1)));
		list[1] = new Sphere(vec3(0, 0, -1.0), R, new Metal(vec3(0.8, 0.6, 0.2), 1.2));
		list[2] = new Sphere(vec3(0.0, -100.5, -1.0), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
		list[3] = new Sphere(vec3(-R, 0.0, -1.0), R, new Dielectric(1.3));
		Hitable* world = new HitableList(list, 4);

		Camera cam(vec3(-1, 1.5, .5), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));
		for(int j = ny - 1; j >= 0; j--)
		{
			for(int i = 0; i < nx; i++)
			{
				vec3 col(0.0, 0.0, 0.0);
				for(int s = 0; s < ns; s++)
				{
					float u = float(i + createRandom()) / float(nx);
					float v = float(j + createRandom()) / float(ny);

					Ray ray = cam.getRay(u, v);
					vec3 p = ray.pFunction(1.0);
					col += setColor(ray, world, 0);
				}
				col /= float(ns);
				col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));
				int ir = int(255.99 * col.x);
				int ig = int(255.99 * col.y);
				int ib = int(255.99 * col.z);
				imageFile << ir << " " << ig << " " << ib << "\n";
			}
		}
		imageFile.close();

		std::cout << "Time is: ";
	}
	return 0;
}