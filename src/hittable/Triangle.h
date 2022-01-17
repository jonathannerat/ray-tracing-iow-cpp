#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class Triangle : public Hittable {
	public:
	Point3 p0, p1, p2;

	shared_ptr<Material> mat_ptr;

	Triangle(Point3 p0, Point3 p1, Point3 p2, shared_ptr<Material> m): p0(p0), p1(p1), p2(p2), mat_ptr(m) {}

	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
};

#endif // TRIANGLE_H
