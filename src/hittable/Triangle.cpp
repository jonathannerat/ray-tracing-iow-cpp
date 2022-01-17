#include "Triangle.h"

bool Triangle::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
	Vec3 normal = cross(p1 - p0, p2 - p0);

	if (perpendicular(normal, r.dir)) return false;

	double t = dot(p0 - r.origin, normal) / dot(r.dir, normal);
	if (t < t_min || t_max < t) return false;

	Point3 p = r(t);

	if (dot(normal, cross(p1 - p0, p - p0)) > 0
	 && dot(normal, cross(p2 - p1, p - p1)) > 0
	 && dot(normal, cross(p0 - p2, p - p2)) > 0) {
		rec.t = t;
		rec.p = p;
		rec.set_face_normal(r, normal);
		rec.mat_ptr = mat_ptr;
		return true;
	} else {
		return false;
	}
}
