#include "Scene.h"

#include <sstream>
#include <optional>
#include <vector>
#include "rtweekend.h"
#include "Vec3.h"
#include "Ray.h"
#include "color.h"
#include "materials/Material.h"
#include "hittable/Sphere.h"
#include "hittable/Box.h"
#include "hittable/Triangle.h"
#include "hittable/TriangleMesh.h"
#include "KDTree.h"

using namespace std;

enum ParsingStage {
    NONE, OUTPUT, CAMERA, MATERIALS, OBJECTS
};

#define IMG_WIDTH 300
#define ASPECT_RATIO (16.0/9.0)
#define SPP 50
#define DEPTH 5
#define FROM Vec3(0,1,-3)
#define TO Vec3()
#define VUP Vec3(0,1,0)
#define VFOV 45
#define APERTURE 0
#define FOCUS ((TO-FROM).length())
#define LEAF_SIZE 25

Vec3 Scene::parse_vec3(const std::string &s) {
    double x, y, z;

    size_t lastpos = s.find('=');
    lastpos = lastpos == string::npos ? 0 : lastpos + 1;
    size_t pos = s.find(',', lastpos);
    x = stod(s.substr(lastpos, pos - lastpos));

    lastpos = pos;
    pos = s.find(',', lastpos + 1);
    y = stod(s.substr(lastpos + 1, pos - lastpos));

    lastpos = pos;
    pos = s.find(',', lastpos + 1);
    z = stod(s.substr(lastpos + 1, pos - lastpos));

    return {x, y, z};
}

Scene::Scene()
        : output(IMG_WIDTH, ASPECT_RATIO, SPP, DEPTH),
          camera(FROM, TO, VUP, VFOV, ASPECT_RATIO, APERTURE, FOCUS) {
    world.add(make_shared<KDTree<>>(random_objects(), LEAF_SIZE));
    auto ground_mat = make_shared<Lambertian>(Color(.5, .5, .5));
    world.add(make_shared<Plane>(Point3(), VUP, ground_mat));
}

Scene::Scene(const string &filepath)
        : output(IMG_WIDTH, ASPECT_RATIO, SPP, DEPTH),
          camera(FROM, TO, VUP, VFOV, ASPECT_RATIO, APERTURE, FOCUS) {
    string line;
    ifstream file(filepath);

    if (!file.is_open())
        throw runtime_error("Failed to open scene file: " + filepath);

    vector<shared_ptr<Material>> materials;
    ParsingStage stage = NONE;

    Vec3 from(0, 1, -1), to, vup(0, 1, 0);
    double focus = (to - from).length(), aperture = 0;
    int fov = 45;

    while (getline(file, line)) {
        // ignorar comentarios y lineas vacias
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string arg;

        if ((stage == OUTPUT && (line[0] == ' ' || line[0] == '\t')) ||
            !line.compare(0, 7, "output:")) {
            if (stage != OUTPUT) {
                stage = OUTPUT;
                ss >> arg;
            }

            while (!ss.eof()) {
                ss >> arg;

                if (!arg.compare(0, 6, "ratio=")) {
                    size_t pos;
                    if ((pos = arg.find('/')) != string::npos) {
                        output.aspect_ratio = stod(arg.substr(6, pos-6)) / stod(arg.substr(pos+1));
                    } else {
                        output.aspect_ratio = stod(arg.substr(6));
                    }
                    output.height = static_cast<int>(output.width / output.aspect_ratio);
                    camera = Camera(from, to, vup, fov, output.aspect_ratio, aperture, focus);
                } else if (!arg.compare(0, 6, "width=")) {
                    output.width = stoi(arg.substr(6));
                    output.height = static_cast<int>(output.width / output.aspect_ratio);
                } else if (!arg.compare(0, 7, "height="))
                    output.aspect_ratio = output.width / stod(arg.substr(7));
                else if (!arg.compare(0, 4, "spp="))
                    output.samples_per_pixel = stoi(arg.substr(4));
                else if (!arg.compare(0, 6, "depth="))
                    output.max_depth = stoi(arg.substr(6));
            }
        } else if ((stage == CAMERA && (line[0] == ' ' || line[0] == '\t')) ||
                   !line.compare(0, 7, "camera:")) {
            if (stage != CAMERA) {
                stage = CAMERA;
                ss >> arg;
            }

            if (!ss.eof()) {
                while (!ss.eof()) {
                    ss >> arg;
                    if (!arg.compare(0, 5, "from=")) {
                        from = parse_vec3(arg);
                        focus = (to - from).length();
                    } else if (!arg.compare(0, 3, "to=")) {
                        to = parse_vec3(arg);
                        focus = (to - from).length();
                    } else if (!arg.compare(0, 4, "vup="))
                        vup = parse_vec3(arg);
                    else if (!arg.compare(0, 6, "focus="))
                        focus = stod(arg.substr(6));
                    else if (!arg.compare(0, 9, "aperture="))
                        aperture = stod(arg.substr(9));
                    else if (!arg.compare(0, 4, "fov="))
                        fov = stoi(arg.substr(4));
                    else
                        throw invalid_argument("Bad argument: " + arg);
                }
                camera = Camera(from, to, vup, fov, output.aspect_ratio, aperture, focus);
            }
        } else if ((stage == MATERIALS && (line[0] == ' ' || line[0] == '\t')) ||
                   !line.compare(0, 10, "materials:")) {
            if (stage != MATERIALS) {
                stage = MATERIALS;
                ss >> arg;
            }

            while (!ss.eof()) {
                ss >> arg;
                if (!arg.compare(0, 11, "dielectric=")) {
                    ss >> arg;
                    materials.push_back(make_shared<Dielectric>(stod(arg.substr(11))));
                } else if (!arg.compare(0, 11, "lambertian=")) {
                    ss >> arg;
                    materials.push_back(make_shared<Lambertian>(parse_vec3(arg)));
                } else if (!arg.compare(0, 6, "metal:")) {
                    Vec3 color(1,1,1);
                    double fuzz = 0;
                    while (!ss.eof()) {
                        ss >> arg;
                        if (!arg.compare(0, 6, "color="))
                            color = parse_vec3(arg);
                        else if (!arg.compare(0, 5, "fuzz="))
                            fuzz = stod(arg.substr(5));
                    }
                    materials.push_back(make_shared<Metal>(color, fuzz));
                } else {
                    throw invalid_argument("Bad argument: " + arg);
                }
            }
        } else if ((stage == OBJECTS && (line[0] == ' ' || line[0] == '\t')) ||
                   !line.compare(0, 8, "objects:")) {
            if (stage != OBJECTS) {
                stage = OBJECTS;
                ss >> arg;
            }

            size_t material = materials.size();

            while (!ss.eof()) {
                ss >> arg;
                if (!arg.compare(0, 7, "sphere:")) {
                    Vec3 center(0,1,0);
                    double radius = 1;

                    while (!ss.eof()) {
                        ss >> arg;
                        if (!arg.compare(0, 7, "center="))
                            center = parse_vec3(arg);
                        else if (!arg.compare(0, 7, "radius="))
                            radius = stod(arg.substr(7));
                        else if (!arg.compare(0, 9, "material="))
                            material = stol(arg.substr(9));
                    }

                    if (material >= materials.size())
                        throw invalid_argument("Bad material index:" + to_string(material));

                    world.add(make_shared<Sphere>(center, radius, materials[material]));
                } else if (!arg.compare(0, 4, "box:")) {
                    Vec3 cback, cfront(1,1,1);

                    while (!ss.eof()) {
                        ss >> arg;
                        if (!arg.compare(0, 6, "cback="))
                            cback = parse_vec3(arg);
                        else if (!arg.compare(0, 7, "cfront="))
                            cfront = parse_vec3(arg);
                        else if (!arg.compare(0, 9, "material="))
                            material = stol(arg.substr(9));
                    }

                    if (material >= materials.size())
                        throw invalid_argument("Bad material index:" + to_string(material));

                    world.add(make_shared<Box>(cback, cfront, materials[material]));
                } else if (!arg.compare(0, 6, "plane:")) {
                    Vec3 origin, normal(0, 1, 0);

                    while (!ss.eof()) {
                        ss >> arg;
                        if (!arg.compare(0, 7, "origin="))
                            origin = parse_vec3(arg);
                        else if (!arg.compare(0, 7, "normal="))
                            normal = parse_vec3(arg);
                        else if (!arg.compare(0, 9, "material="))
                            material = stol(arg.substr(9));
                    }

                    if (material >= materials.size())
                        throw invalid_argument("Bad material index:" + to_string(material));

                    world.add(make_shared<Plane>(origin, normal, materials[material]));
                } else if (!arg.compare(0, 5, "mesh:")) {
                    string objpath;
                    size_t leafs = 100;
                    Vec3 move, scale;

                    while (!ss.eof()) {
                        ss >> arg;
                        if (!arg.compare(0, 8, "objpath="))
                            objpath = arg.substr(8);
                        else if (!arg.compare(0, 6, "leafs="))
                            leafs = stol(arg.substr(6));
                        else if (!arg.compare(0, 9, "material="))
                            material = stol(arg.substr(9));
                        else if (!arg.compare(0, 5, "move="))
                            move = parse_vec3(arg);
                        else if (!arg.compare(0, 6, "scale="))
                            scale = parse_vec3(arg);
                    }

                    if (material >= materials.size())
                        throw invalid_argument("Bad material index:" + to_string(material));

                    auto mesh = TriangleMesh(objpath, materials[material]);
                    if (!scale.near_zero()) mesh.scale(scale);
                    if (!move.near_zero()) mesh.move(move);

                    world.add(make_shared<KDTree<Triangle>>(mesh, leafs));
                } else
                    throw invalid_argument("Bad argument: " + arg);
            }
        } else if (stage != NONE) {
            throw invalid_argument("Bad line: " + line);
        }
    }

    if (world.size() == 0) {
        world.add(make_shared<KDTree<>>(random_objects(), 25));
        auto ground_mat = make_shared<Lambertian>(Color(.5, .5, .5));
        world.add(make_shared<Plane>(Point3(), Vec3(0, 1, 0), ground_mat));
    }
}

void Scene::render(ostream &os) const {
    os << "P3\n" << output.width << ' ' << output.height << "\n255\n";

    for (int j = output.height - 1; j >= 0; j--) {
        cout << "\rWriting lines: " << output.height - j << '/' << output.height << "     "
             << flush;
        for (int i = 0; i < output.width; i++) {
            Color pixel;

            for (int s = 0; s < output.samples_per_pixel; s++) {
                double u = (double) (i + random_double()) / (output.width - 1);
                double v = (double) (j + random_double()) / (output.height - 1);
                Ray r = camera.get_ray(u, v);
                pixel += ray_color(r, world, output.max_depth);
            }

            write_color(os, pixel, output.samples_per_pixel);
        }
    }

    cout << "\nDone!" << endl;
}

Color Scene::ray_color(const Ray &r, const Hittable &world, int depth) {
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

HittableList<> Scene::random_objects() {
    HittableList<> scene;

    auto mat1 = make_shared<Dielectric>(1.5);
    auto sphere1 = make_shared<Sphere>(Point3(-2, 1, 0), 1, mat1);
    scene.add(sphere1);

    auto mat3 = make_shared<Metal>(Color(.7, .6, .5), .05);
    auto sphere2 = make_shared<Sphere>(Point3(2, 1, 0), 1, mat3);
    scene.add(sphere2);

    auto mat_cow = make_shared<Lambertian>(Color(1, .84, .36));
    auto cow = make_shared<KDTree<Triangle>>(TriangleMesh("models/cow.obj", mat_cow), 200);
    cow->scale(Vec3(3, 3, 3));
    cow->move(Vec3(0, 1, 0));
    scene.add(cow);

    for (int a = -5; a < 16; a++) {
        for (int b = -5; b < 16; b++) {
            double rnd_mat = random_double();
            double rnd_obj = random_double();
            Point3 center(a + .7 * random_double(), 0, b + .7 * random_double());
            shared_ptr<Material> mat;

            // guardar un espacio alrededor de los objetos destacados
            if ((center - sphere1->refp).length() < 1.5 ||
                (center - sphere2->refp).length() < 1.5 ||
                (center - cow->refp).length() < 2)
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

            if (rnd_obj < .7) {
                // sphere
                double radius = random_double(.15, .25);
                center.y = radius;
                scene.add(make_shared<Sphere>(center, radius, mat));
            } else {
                // box
                double halfside = random_double(.15, .25);
                center.y = halfside;
                scene.add(make_shared<Box>(center - halfside, center + halfside, mat));
            }
        }
    }

    return scene;
}
