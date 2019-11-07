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
	if(world->hit(ray, 0.001, FLT_MAX, record))
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

Hitable* randomScene()
{
	int n = 500;
	Hitable** list = new Hitable * [n + 1];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for(int a = -11; a < 11; a++)
	{
		for(int b = -11; b < 11; b++)
		{
			float chooseMat = randomDouble();
			vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
			if((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if(chooseMat < 0.8) // diffuse
				{
					list[i++] = new Sphere(center, 0.2,
						new Lambertian(vec3(randomDouble() * randomDouble(),
							randomDouble() * randomDouble(),
							randomDouble() * randomDouble())));
				}
				else if(chooseMat < 0.95) // metal
				{
					list[i++] = new Sphere(center, 0.2, new Metal(
						vec3(0.5 * (1 + randomDouble()),
							0.5 * (1 + randomDouble()),
							0.5 * (1 + randomDouble())),
						0.5 * randomDouble()));
				}
				else // glass
				{
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
}

int main()
{
	// Calculate Time for running the code
	// inline functions and regular functions.
	{
		Timer timer;
		int nx = 1200;
		int ny = 800;
		int ns = 10;

		std::ofstream imageFile;
		imageFile.open("../output/outputImage.ppm");

		imageFile << "P3\n" << nx << " " << ny << "\n255\n";

		Hitable* world = randomScene();

		vec3 lookFrom(13, 2, 3);
		vec3 lookAt(0, 0, 0);
		float aperture = 0.1;
		float dof = 10.0;
		Camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dof);
		for(int j = ny - 1; j >= 0; j--)
		{
			for(int i = 0; i < nx; i++)
			{
				vec3 col(0.0, 0.0, 0.0);
				for(int s = 0; s < ns; s++)
				{
					float u = float(i + randomDouble()) / float(nx);
					float v = float(j + randomDouble()) / float(ny);

					Ray ray = cam.getRay(u, v);
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