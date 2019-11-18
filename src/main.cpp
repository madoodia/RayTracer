// madoodia@gmail.com
// (C) 2019
// All rights reserved.
// ------------------

// PPM Viewer: http://web.eecs.utk.edu/~smarz1/pgmview/

// C++ Headers
#include <iostream>
#include <fstream>
#include <float.h>

// Third Party Headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Own Headers
#include "common.h"
#include "myTimer.h"
#include "sphere.h"
#include "hittableList.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "texture.h"
#include "rectangle.h"
#include "flipNormal.h"
#include "box.h"
#include "transformation.h"
#include "volume.h"
#include "pdf.h"

inline vec3 deNAN(const vec3 &c)
{
	vec3 temp = c;
	if (!(temp[0] == temp[0]))
		temp[0] = 0;
	if (!(temp[1] == temp[1]))
		temp[1] = 0;
	if (!(temp[2] == temp[2]))
		temp[2] = 0;
	return temp;
}

vec3 setColor(const Ray &ray, Hittable *world, Hittable *lightShape, int depth)
{
	HitRecord hrec;
	if (world->hit(ray, 0.001, FLT_MAX, hrec))
	{
		ScatterRecord srec;
		vec3 emitted = hrec.matPtr->emitted(ray, hrec, hrec.u, hrec.v, hrec.p);
		if (depth < 50 && hrec.matPtr->scatter(ray, hrec, srec))
		{
			if (srec.isSpecular)
			{
				return srec.attenuation *
					   setColor(srec.specularRay, world, lightShape, depth + 1);
			}
			else
			{
				HittablePDF plight(lightShape, hrec.p);
				MixturePDF p(&plight, srec.pdfPtr);
				Ray scattered = Ray(hrec.p, p.generate(), ray.time());
				float pdfValue = p.value(scattered.direction());
				delete srec.pdfPtr;
				return emitted +
					   srec.attenuation *
						   hrec.matPtr->scatteringPDF(ray, hrec, scattered) *
						   setColor(scattered, world, lightShape, depth + 1) / pdfValue;
			}
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3(0, 0, 0);
	}
}

Hittable *cornellBox4Scene()
{
	int i = 0;
	Hittable **list = new Hittable *[8];
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));
	list[i++] = new FlipNormal(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new FlipNormal(new XZRect(213, 343, 227, 332, 554, light));
	list[i++] = new FlipNormal(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormal(new XYRect(0, 555, 0, 555, 555, white));
	Material *glass = new Dielectric(1.5);
	list[i++] = new Sphere(vec3(190, 90, 190), 90, glass);
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295));

	return new HittableList(list, i);
}

// main entry point
int main()
{
	// Calculate Time for running the code
	// inline functions and regular functions.
	{
		Timer timer;
		int nx = 500;
		int ny = 500;
		int ns = 100; // samples

		std::cout << "Width: " << nx << "\nHeight: " << ny << "\nSamples: " << ns << std::endl;
		std::cout << "\nRendering...\n";

		std::ofstream imageFile;
		imageFile.open("output/outputImage.ppm");

		imageFile << "P3\n"
				  << nx << " " << ny << "\n255\n";

		Hittable *world = cornellBox4Scene();

		vec3 lookFrom(278, 278, -800);
		vec3 lookAt(278, 278, 0);
		float aperture = 0.0; // no blur
		float dof = 10.0;
		float vfov = 40.0;
		Camera cam(lookFrom, lookAt,
				   vec3(0, 1, 0), vfov,
				   float(nx) / float(ny),
				   aperture, dof,
				   0.0, 1.0);

		Hittable *lightShape = new XZRect(213, 343, 227, 332, 554, 0);
		Hittable *glassSphere = new Sphere(vec3(190, 90, 190), 90, 0);

		Hittable *a[2];
		a[0] = lightShape;
		a[1] = glassSphere;
		HittableList hlist(a, 2);

		for (int j = ny - 1; j >= 0; j--)
		{
			for (int i = 0; i < nx; i++)
			{
				vec3 col(0.0, 0.0, 0.0);
				for (int s = 0; s < ns; s++)
				{
					float u = float(i + randomDouble()) / float(nx);
					float v = float(j + randomDouble()) / float(ny);

					Ray ray = cam.getRay(u, v);
					vec3 p = ray.pFunction(2.0);
					col += deNAN(setColor(ray, world, &hlist, 0));
				}
				col /= float(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				imageFile << ir << " " << ig << " " << ib << "\n";
			}
		}
		imageFile.close();

		std::cout << "Time is: ";
	}

	std::system("pause");

	return 0;
}
