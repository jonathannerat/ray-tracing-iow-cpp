#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
	public:
	Point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
	
	Sphere() {};
	Sphere(Point3 c, double r, shared_ptr<Material> m): center(c), radius(r), mat_ptr(m) {};

	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
};

#endif // SPHERE_H
