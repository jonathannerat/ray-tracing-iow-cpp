#include "Sphere.h"

using std::sqrt;

bool Sphere::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
	Vec3 oc = r.origin - center;
	double a = r.dir.length2();
	double hb = dot(oc, r.dir);
	double c = oc.length2() - radius*radius;

	double discriminant = hb * hb - a * c;
	if (discriminant < 0) return false;

	double sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	double root = (-hb - sqrtd) / a;
	if (root < t_min || t_max < root) {
			root = (-hb + sqrtd) / a;
			if (root < t_min || t_max < root)
					return false;
	}

	rec.t = root;
	rec.p = r(rec.t);
	Vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}
