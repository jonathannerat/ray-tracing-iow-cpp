#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"

class Camera {
	public:
		Camera() {
			double aspect_ratio = 16.0 / 9.0;
			double vp_height = 2.0;
			double vp_width = aspect_ratio * vp_height;
			double focal_length = 1.0;

			origin = Point3();
			horizontal = Vec3(vp_width, 0, 0);
			vertical = Vec3(0, vp_height, 0);
			bl_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);
		}

		Ray get_ray(double u, double v) const {
			return Ray(origin, bl_corner + u*horizontal + v*vertical - origin);
	}
	private:
		Point3 origin;
		Point3 bl_corner;
		Vec3 horizontal;
		Vec3 vertical;
};

#endif // CAMERA_Hragma
