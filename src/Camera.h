#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Vec3.h"

class Camera {
public:
    Camera() = default;
    Camera(Point3 from, Point3 to, Vec3 vup, double vfov, double aspect_ratio,
           double aperture, double focus_dist) {
        double theta = degrees_to_radians(vfov);
        double h = tan(theta / 2);
        double vp_height = 2.0 * h;
        double vp_width = aspect_ratio * vp_height;

        w = (from - to).normalized();
        u = cross(vup, w).normalized();
        v = cross(w, u);

        origin = from;
        horizontal = focus_dist * vp_width * u;
        vertical = focus_dist * vp_height * v;
        bl_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;
    }

    Ray get_ray(double s, double t) const {
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x + v * rd.y;
        return Ray(origin + offset,
                   bl_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    Point3 origin;
    Point3 bl_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lens_radius = 1;
};

#endif // CAMERA_H
