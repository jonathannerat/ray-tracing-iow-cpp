#include "rtweekend.h"
#include "Vec3.h"
#include "Ray.h"
#include "Camera.h"
#include "hittable/Sphere.h"
#include "materials/Material.h"
#include "hittable/HittableList.h"
#include "color.h"

#include <iostream>

using namespace std;

Color ray_color(const Ray &r, const Hittable &world, int depth);

int
main(int argc, char **argv) {
	// image
	const double aspect_ratio = 16.0 / 9.0;
	const int img_width = 400;
	const int img_height = static_cast<int>(img_width / aspect_ratio);
	const int spp = 100;
	const int max_depth = 50;

	// world
	HittableList world;
	auto material_ground = make_shared<Lambertian>(Color(.8, .8, 0));
	auto material_center = make_shared<Lambertian>(Color(.7, .3, .3));
	auto material_left = make_shared<Metal>(Color(.8, .8, .8));
	auto material_right = make_shared<Metal>(Color(.8, .6, .2));

	world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, material_ground));
	world.add(make_shared<Sphere>(Point3(0, 0, -1), .5, material_center));
	world.add(make_shared<Sphere>(Point3(-1, 0, -1), .5, material_left));
	world.add(make_shared<Sphere>(Point3(1, 0, -1), .5, material_right));

	// camera
	Camera cam;

	// render
	cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

	cerr << "Scanlines remaining: ";
	for (int j = img_height - 1; j >= 0; j--) {
		cerr << j << ' ';
		for (int i = 0; i < img_width; i++) {
			Color pixel;

			for (int s = 0; s < spp; s++) {
				double u = (double) (i + random_double()) / (img_width - 1);
				double v = (double) (j + random_double()) / (img_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel += ray_color(r, world, max_depth);
			}

			write_color(cout, pixel, spp);
		}
	}
	cerr << ". Done.\n";

	return 0;
}


Color
ray_color(const Ray &r, const Hittable &world, int depth) {
	if (depth <= 0) return Color();

	HitRecord rec;

	if (world.hit(r, 0.001, infinity, rec)) {
		Ray scattered;
		Color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return Color();
	}

	Vec3 unit_dir = r.dir.normalized();
	double t = .5 * (unit_dir.y + 1);
	
	return (1 - t) * Color(1, 1, 1) + t * Color(.5, .7, 1);
}
