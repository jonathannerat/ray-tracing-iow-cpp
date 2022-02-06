#ifndef RAYTRACINGIOW_SCENE_H
#define RAYTRACINGIOW_SCENE_H

#include <string>
#include <fstream>

#include "Vec3.h"
#include "Camera.h"
#include "hittable/HittableList.h"

struct Output {
    double aspect_ratio;
    int width;
    int height;
    int samples_per_pixel;
    int max_depth;

    Output() = default;
    Output(int width, double aspect_ratio, int samples_per_pixel, int max_depth)
            : width(width), aspect_ratio(aspect_ratio), samples_per_pixel(samples_per_pixel),
              height(static_cast<int>(width / aspect_ratio)), max_depth(max_depth) {};
};

class Scene {
private:
    void render(std::ostream &s) const;
public:
    Scene();
    Scene(const std::string &filepath);

    void render(const std::string &filepath) const {
        std::ofstream file(filepath);
        render(file);
    }

    void render() const { render(std::cout); }

private:
    static Vec3 parse_vec3(const std::string &s);
    static Color ray_color(const Ray &r, const Hittable &world, int depth);
    static HittableList<> random_objects();

    Camera camera;
    Output output;
    HittableList<> world;
};

#endif //RAYTRACINGIOW_SCENE_H
