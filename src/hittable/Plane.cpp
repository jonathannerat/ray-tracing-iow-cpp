#include "Plane.h"

#include "math.h"

bool Plane::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
	if (perpendicular(r.dir, normal))
		return false; // el rayo es paralelo al plano, consideramos que no se cruzan

	double t = dot(origin - r.origin, normal) / dot(r.dir, normal);
	if (t < t_min || t_max < t) return false;

	rec.t = t;
	rec.p = r(t);
	rec.set_face_normal(r, normal);
	rec.mat_ptr = mat_ptr;

	return true;
}
