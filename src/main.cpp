#include "Camera.h"
#include "KDTree.h"
#include "Ray.h"
#include "Vec3.h"
#include "color.h"
#include "hittable/Plane.h"
#include "hittable/Sphere.h"
#include "hittable/Triangle.h"
#include "hittable/TriangleMesh.h"
#include "hittable/HittableList.h"
#include "materials/Material.h"
#include "rtweekend.h"

#include <iostream>

using namespace std;

Color ray_color(const Ray &r, const Hittable &world, int depth);

HittableList<> random_scene();

int main(int argc, char **argv) {
    // image
    const double aspect_ratio = 3.0 / 2.0;
    const int img_width = 400;
    const int img_height = static_cast<int>(img_width / aspect_ratio);
    const int spp = 50;
    const int max_depth = 20;

    // world
    HittableList world = random_scene();

    // camera
    Point3 from(-10, 6, -12);
    Point3 to(0, 2, 0);
    Vec3 vup(0, 1, 0);
    double dist_to_focus = 10;
    double aperture = .1;
    int field_of_view = 45;
    Camera cam(from, to, vup, field_of_view, aspect_ratio, aperture, dist_to_focus);

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

Color ray_color(const Ray &r, const Hittable &world, int depth) {
    if (depth <= 0) return {};

    HitRecord rec;

    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return {};
    }

    // Gradiente
    Vec3 unit_dir = r.dir.normalized();
    double t = .5 * (unit_dir.y + 1);
    Color bg_color = (1 - t) * Color(1, 1, 1) + t * Color(.5, .7, 1);

    return bg_color;
}

HittableList<> random_scene() {
    HittableList<> scene;

    auto ground_mat = make_shared<Lambertian>(Color(.5, .5, .5));
    scene.add(make_shared<Plane>(Point3(), Vec3(0, 1, 0), ground_mat));

    for (int a = -10; a < 11; a += 2) {
        for (int b = -10; b < 11; b += 2) {
            double rnd_mat = random_double();
            double rnd_obj = random_double();
            Point3 center(a + 1.9 * random_double(), .5, b + 1.9 * random_double());
            shared_ptr<Material> mat;

            if ((center - Point3(0, 3, 0)).length() < 4 ||
                (center - Point3(6, 3, 0)).length() < 4 ||
                (center - Point3(-6, 3, 0)).length() < 4)
                continue;

            if (rnd_mat < .8) {
                // diffuse
                mat = make_shared<Lambertian>(Color::random() * Color::random());
            } else if (rnd_mat < .95) {
                // metal
                mat = make_shared<Metal>(Color::random(.5, 1), random_double(0, .5));
            } else {
                // glass
                mat = make_shared<Dielectric>(1.5);
            }

            if (rnd_obj < .5) {
                // sphere
                double radius = random_double(.2, .5);
                center.y = radius;
                scene.add(make_shared<Sphere>(center, radius, mat));
            } else {
                // box
                double side = random_double(.5, 1);
                center.y = side / 2;
                scene.add(make_shared<Box>(center - side / 2, center + side / 2, mat));
            }
        }
    }

    auto mat1 = make_shared<Dielectric>(1.5);
    scene.add(make_shared<Sphere>(Point3(-6, 3, 0), 3, mat1));

    auto mat2 = make_shared<Lambertian>(Color(.5, .3, .9));
    scene.add(make_shared<Box>(Point3(-2, 5, 0), Point3(2, 9, 4), mat2));

    auto mat3 = make_shared<Metal>(Color(.7, .6, .5), .05);
    scene.add(make_shared<Sphere>(Point3(6, 3, 0), 3, mat3));

    auto mat_cow = make_shared<Lambertian>(Color(1, .84, .36));
    auto cow = TriangleMesh("models/cow.obj", mat_cow);
    cow.scale(Vec3(2, 2, 2));
    cow.move(Vec3(0, 2, 0));
    scene.add(make_shared<KDTree<Triangle>>(cow, 200));


//    auto duck = TriangleMesh("models/duck.obj", cow_mat);
//    auto kdduck = make_shared<KDTree<Triangle>>(duck, 200);
//    auto sphere = make_shared<Sphere>(Point3(-1, 1, -1), .5, sphere_mat);
//    auto sphere_di = make_shared<Sphere>(Point3(1, 1, -1), .5, sphere_mat_di);

    return scene;
}
