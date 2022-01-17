#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"

class Plane : public Hittable {
	public:
	Point3 origin;
	Vec3 normal;
	shared_ptr<Material> mat_ptr;

	Plane(Point3 o, Vec3 n, shared_ptr<Material> m): origin(o), normal(n), mat_ptr(m) {};

	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
};

#endif // PLANE_H
