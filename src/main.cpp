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

vec3 setColor(const Ray &ray, Hittable *world, int depth)
{
	HitRecord record;
	if (world->hit(ray, 0.001, FLT_MAX, record))
	{
		Ray scattered;
		vec3 attenuation;
		vec3 emitted = record.matPtr->emitted(record.u, record.v, record.p);
		float pdf;
		vec3 albedo;
		if (depth < 50 && record.matPtr->scatter(ray, record, albedo, scattered, pdf))
		{
			return emitted + albedo * record.matPtr->scatteringPDF(ray, record, scattered) * setColor(scattered, world, depth + 1) / pdf; // recursive
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

	// else
	// {
	// 	vec3 unit = ray.direction().normalize();
	// 	float t = 0.5 * (unit.y + 1.0);
	// 	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	// }
}

Hittable *randomScene()
{
	int n = 50000;
	Hittable **list = new Hittable *[n + 1];
	Texture *checker = new CheckerTexture(
		new ConstantTexture(vec3(0.2, 0.3, 0.1)),
		new ConstantTexture(vec3(0.9, 0.9, 0.9)));
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(checker));

	int i = 1;
	for (int a = -10; a < 10; a++)
	{
		for (int b = -10; b < 10; b++)
		{
			float chooseMat = randomDouble();
			vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (chooseMat < 0.5) // diffuse - motion blur
				{
					list[i++] = new MovingSphere(center, center + vec3(0, 0.5 * randomDouble(), 0),
												 0.0, 1.0, 0.2,
												 new Lambertian(new ConstantTexture(
													 vec3(randomDouble() * randomDouble(),
														  randomDouble() * randomDouble(),
														  randomDouble() * randomDouble()))));
				}
				if (chooseMat < 0.8) // diffuse
				{
					list[i++] = new Sphere(center, 0.2,
										   new Lambertian(new ConstantTexture(
											   vec3(randomDouble() * randomDouble(),
													randomDouble() * randomDouble(),
													randomDouble() * randomDouble()))));
				}
				else if (chooseMat < 0.95) // metal
				{
					list[i++] = new Sphere(center, 0.2,
										   new Metal(
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
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

	// return new HittableList(list, i);
	return new bvhNode(list, i, 0.0, 1.0); // using BVH method
}

Hittable *twoSpheresScene()
{
	Texture *checker = new CheckerTexture(
		new ConstantTexture(vec3(0.2, 0.3, 0.1)),
		new ConstantTexture(vec3(0.9, 0.9, 0.9)));
	int n = 2;
	Hittable **list = new Hittable *[n + 1];
	list[0] = new Sphere(vec3(0, -10, 0), 10, new Lambertian(checker));
	list[1] = new Sphere(vec3(0, 10, 0), 10, new Lambertian(checker));

	return new HittableList(list, 2);
	// return new bvhNode(list, 2, 0.0, 1.0); // using BVH method
}

Hittable *twoPerlinSpheresScene()
{
	Texture *perlinTexture = new NoiseTexture(2.5);
	Hittable **list = new Hittable *[2];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(perlinTexture));
	list[1] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(perlinTexture));

	return new HittableList(list, 2);
	// return new bvhNode(list, 2, 0.0, 1.0); // using BVH method
}

Hittable *earthScene()
{
	int nx, ny, nn;
	//unsigned char *texData = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
	unsigned char *texData = stbi_load("src/earthmap.jpg", &nx, &ny, &nn, 0);
	Material *mat = new Lambertian(new ImageTexture(texData, nx, ny));
	return new Sphere(vec3(0, 0, 0), 2, mat);
}

Hittable *moonScene()
{
	int nx, ny, nn;
	//unsigned char *texData = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
	unsigned char *texData = stbi_load("src/moonmap.jpg", &nx, &ny, &nn, 0);
	Material *mat = new Lambertian(new ImageTexture(texData, nx, ny));
	return new Sphere(vec3(0, 0, 0), 2, mat);
}

Hittable *simpleLightScene()
{
	Texture *pertext = new NoiseTexture(4);
	Hittable **list = new Hittable *[4];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(pertext));
	list[1] = new Sphere(vec3(0, 2, 0), 2, new Lambertian(pertext));
	list[2] = new Sphere(vec3(0, 7, 0), 2,
						 new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
	list[3] = new XYRect(3, 5, 1, 3, -2,
						 new DiffuseLight(new ConstantTexture(vec3(4, 4, 4))));
	return new HittableList(list, 4);
}

Hittable *cornellBoxScene()
{
	Hittable **list = new Hittable *[6];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));

	list[i++] = new FlipNormal(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new FlipNormal(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormal(new XYRect(0, 555, 0, 555, 555, white));

	return new HittableList(list, i);
	// return new bvhNode(list, i, 0.0, 1.0); // using BVH method (took longer)
}

Hittable *cornellBox2Scene()
{
	Hittable **list = new Hittable *[6];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));

	list[i++] = new FlipNormal(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new FlipNormal(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormal(new XYRect(0, 555, 0, 555, 555, white));

	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0),
							vec3(165, 165, 165), white),
					-18),
		vec3(130, 0, 65));
	list[i++] = new Translate(
		new RotateY(new Box(vec3(0, 0, 0),
							vec3(165, 330, 165), white),
					15),
		vec3(265, 0, 295));

	return new HittableList(list, i);
	// return new bvhNode(list, i, 0.0, 1.0); // using BVH method (took longer)
}

Hittable *cornellSmokeScene()
{
	Hittable **list = new Hittable *[8];
	int i = 0;
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(10, 10, 10)));

	list[i++] = new FlipNormal(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(113, 443, 127, 432, 554, light);
	list[i++] = new FlipNormal(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormal(new XYRect(0, 555, 0, 555, 555, white));

	Hittable *b1 = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
		vec3(130, 0, 65));
	Hittable *b2 = new Translate(
		new RotateY(new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
		vec3(265, 0, 295));

	list[i++] = new ConstantMedium(
		b1, 0.01, new ConstantTexture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new ConstantMedium(
		b2, 0.01, new ConstantTexture(vec3(0.0, 0.0, 0.0)));

	return new HittableList(list, i);
}

Hittable *finalScene()
{
	int nb = 20;
	Hittable **list = new Hittable *[30];
	Hittable **boxlist = new Hittable *[10000];
	Hittable **boxlist2 = new Hittable *[10000];
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *ground = new Lambertian(new ConstantTexture(vec3(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (randomDouble() + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new Box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvhNode(boxlist, b, 0, 1);
	Material *light = new DiffuseLight(new ConstantTexture(vec3(7, 7, 7)));
	list[l++] = new XZRect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[l++] = new MovingSphere(center, center + vec3(30, 0, 0),
								 0, 1, 50, new Lambertian(new ConstantTexture(vec3(0.7, 0.3, 0.1))));
	list[l++] = new Sphere(vec3(260, 150, 45), 50, new Dielectric(1.5));
	list[l++] = new Sphere(vec3(0, 150, 145), 50,
						   new Metal(vec3(0.8, 0.8, 0.9), 10.0));
	Hittable *boundary = new Sphere(vec3(360, 150, 145), 70, new Dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2,
								   new ConstantTexture(vec3(0.2, 0.4, 0.9)));
	boundary = new Sphere(vec3(0, 0, 0), 5000, new Dielectric(1.5));
	list[l++] = new ConstantMedium(boundary, 0.0001,
								   new ConstantTexture(vec3(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("src/earthmap.jpg", &nx, &ny, &nn, 0);
	Material *emat = new Lambertian(new ImageTexture(tex_data, nx, ny));
	list[l++] = new Sphere(vec3(400, 200, 400), 100, emat);
	Texture *pertext = new NoiseTexture(0.1);
	list[l++] = new Sphere(vec3(220, 280, 300), 80, new Lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxlist2[j] = new Sphere(
			vec3(165 * randomDouble(), 165 * randomDouble(), 165 * randomDouble()),
			10, white);
	}
	list[l++] = new Translate(new RotateY(
								  new bvhNode(boxlist2, ns, 0.0, 1.0), 15),
							  vec3(-100, 270, 395));
	return new HittableList(list, l);
}

Hittable *cornellBox3Scene()
{
	int i = 0;
	Hittable **list = new Hittable *[8];
	Material *red = new Lambertian(new ConstantTexture(vec3(0.65, 0.05, 0.05)));
	Material *white = new Lambertian(new ConstantTexture(vec3(0.73, 0.73, 0.73)));
	Material *green = new Lambertian(new ConstantTexture(vec3(0.12, 0.45, 0.15)));
	Material *light = new DiffuseLight(new ConstantTexture(vec3(15, 15, 15)));
	list[i++] = new FlipNormal(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new FlipNormal(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormal(new XYRect(0, 555, 0, 555, 555, white));
	list[i++] = new Translate(new RotateY(
								  new Box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
							  vec3(130, 0, 65));
	list[i++] = new Translate(new RotateY(
								  new Box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
							  vec3(265, 0, 295));
	return new HittableList(list, i);
}

#if 0
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

		// Hittable *world = randomScene();
		// Hittable *world = twoSpheresScene();
		// Hittable *world = twoPerlinSpheresScene();
		// Hittable *world = earthScene();
		// Hittable *world = moonScene();
		// Hittable *world = simpleLightScene();
		// Hittable *world = cornellBoxScene();
		// Hittable *world = cornellBox2Scene();
		// Hittable *world = cornellSmokeScene();
		// Hittable *world = finalScene();
		Hittable *world = cornellBox3Scene();

		// vec3 lookFrom(13, 2, 3);
		vec3 lookFrom(278, 278, -800);
		vec3 lookAt(278, 278, 0);
		// float aperture = 0.1;
		// float aperture = 0.5; // more blury
		float aperture = 0.0; // no blur
		float dof = 10.0;
		float vfov = 40.0;
		Camera cam(lookFrom, lookAt,
				   vec3(0, 1, 0), vfov,
				   float(nx) / float(ny),
				   aperture, dof,
				   0.0, 1.0);
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

	std::system("pause");

	return 0;
}
#endif

// ----------------------

#if 0
// Monte Carlo (MC) Program Estimate Number P
// A simple MC - 1
#include <iostream>

int main()
{
	int N = 1000;
	int insideCircle = 0;
	for (int i = 0; i < N; i++)
	{
		float x = 2 * randomDouble() - 1;
		float y = 2 * randomDouble() - 1;
		if (x * x + y * y < 1)
			insideCircle++;
	}
	std::cout << "Estimated of Pi = " << 4 * float(insideCircle) / N << "\n";
	return 0;
}
#endif
// ----------------------
#if 0
// A simple MC - 2
#include <iostream>

int main()
{
	int insideCircle = 0;
	int runs = 0;
	while (true)
	{
		runs++;
		float x = 2 * randomDouble() - 1;
		float y = 2 * randomDouble() - 1;
		if (x * x + y * y < 1)
			insideCircle++;

		if (runs % 100000 == 0)
			std::cout << "Estimate of Pi = " << 4 * float(insideCircle) / runs << "\n";
	}
}
#endif

// ----------------------
#if 0
// A simple MC - 3
#include <iostream>

int main()
{
	int insideCircle = 0;
	int insideCircleStratified = 0;
	int sqrtN = 10000;
	for (int i = 0; i < sqrtN; i++)
	{
		for (int j = 0; j < sqrtN; j++)
		{
			float x = 2 * randomDouble() - 1;
			float y = 2 * randomDouble() - 1;
			if (x * x + y * y < 1)
				insideCircle++;
			x = 2 * ((i + randomDouble()) / sqrtN) - 1;
			y = 2 * ((j + randomDouble()) / sqrtN) - 1;
			if (x * x + y * y < 1)
				insideCircleStratified++;
		}
	}
	std::cout.precision(19); // to get more precision in print
	std::cout << "Regular    Estimate of Pi = " << 4 * float(insideCircle) / (sqrtN * sqrtN) << "\n";
	std::cout << "Stratified Estimate of Pi = " << 4 * float(insideCircleStratified) / (sqrtN * sqrtN) << "\n";
}

#endif
// ----------------------
#if 0
// One Dimensional MC Integration 1
#include <iostream>

int main()
{
	int N = 1000000;
	float sum;
	for (int i = 0; i < N; i++)
	{
		float x = 2 * randomDouble();
		sum += x * x;
	}
	std::cout.precision(19); // to get more precision in print
	std::cout << "I =" << 2 * sum / N << "\n";
}
#endif
// ----------------------
#if 0
// One Dimensional MC Integration 2
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"

inline float pdf(float x)
{
	return 0.5 * x;
}

int main()
{
	int N = 1000000;
	float sum;
	for (int i = 0; i < N; i++)
	{
		float x = sqrt(4 * randomDouble());
		sum += x * x / pdf(x);
	}
	std::cout.precision(19); // to get more precision in print
	std::cout << "I =" << sum / N << "\n"; // Nan
}
#endif
// ----------------------
#if 0
// One Dimensional MC Integration 3
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"

inline float pdf(float x)
{
	return 0.5;
}

int main()
{
	int N = 1000000;
	float sum;
	for (int i = 0; i < N; i++)
	{
		float x = 2 * randomDouble();
		sum += x * x / pdf(x);
	}
	std::cout.precision(19); // to get more precision in print
	std::cout << "I =" << sum / N << "\n";
}
#endif
// ----------------------
#if 0
// One Dimensional MC Integration 4
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"

inline float pdf(float x)
{
	return 3 * x * x / 8;
}

int main()
{
	int N = 1;
	float sum;
	for (int i = 0; i < N; i++)
	{
		float x = pow(8 * randomDouble(), 1. / 3.);
		sum += x * x / pdf(x);
	}
	std::cout.precision(19); // to get more precision in print
	std::cout << "I =" << sum / N << "\n";
}
#endif
// ----------------------
#if 0
// MC Integration on the Sphere of Directions 1
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"

inline float pdf(const vec3 &p)
{
	return 1 / (4 * M_PI);
}

int main()
{
	int N = 1000000;
	float sum;
	for (int i = 0; i < N; i++)
	{
		vec3 d = randomOnSphere();
		float cosineSquared = d.z * d.z;
		sum += cosineSquared / pdf(d);
	}
	std::cout << "I =" << sum / N << "\n";
}
#endif
// ----------------------
#if 0
// Generating Random Directions 1
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"
int main()
{
	for (int i = 0; i < 200; i++)
	{
		float r1 = randomDouble();
		float r2 = randomDouble();
		float x = cos(2 * M_PI * r1) * 2 * sqrt(r2 * (1 - r2));
		float y = sin(2 * M_PI * r1) * 2 * sqrt(r2 * (1 - r2));
		float z = 1 - 2 * r2;

		std::cout << x << " " << y << " " << z << "\n";
	}
}
#endif
// ----------------------
#if 0
// Generating Random Directions 2
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"
int main()
{
	int N = 1000000;
	float sum = 0.0;
	for (int i = 0; i < N; i++)
	{
		float r1 = randomDouble();
		float r2 = randomDouble();
		float x = cos(2 * M_PI * r1) * 2 * sqrt(r2 * (1 - r2));
		float y = sin(2 * M_PI * r1) * 2 * sqrt(r2 * (1 - r2));
		float z = 1 - r2;
		sum += z * z * z / (1.0 / (2.0 * M_PI));
	}
	std::cout << "PI/2 = " << M_PI / 2 << "\n";
	std::cout << "Estimate = " << sum / N << "\n";
}
#endif
// ----------------------
#if 1
// Generating Random Directions 3
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "common.h"

inline vec3 randomCosineDirection()
{
	float r1 = randomDouble();
	float r2 = randomDouble();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * sqrt(r2);
	float y = sin(phi) * sqrt(r2);
	return vec3(x, y, z);
}

int main()
{
	int N = 1000000;
	float sum = 0.0;
	for (int i = 0; i < N; i++)
	{
		vec3 v = randomCosineDirection();
		sum += v.z * v.z * v.z / (v.z / (M_PI));
	}
	std::cout << "PI/2 = " << M_PI / 2 << "\n";
	std::cout << "Estimate = " << sum / N << "\n";
}
#endif