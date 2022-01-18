#ifndef BOX_H
#define BOX_H

#include <vector>

#include "hittable.h"
#include "Plane.h"

class Box : public Hittable {
	public:
	Point3 corner1, corner2;
	shared_ptr<Material> mat_ptr;

	Box(Point3 c1, Point3 c2, shared_ptr<Material> m);

	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;

	private:
	std::vector<Plane> faces;

	bool is_inside(const Point3 &p) const;
};

#endif // BOX_H
