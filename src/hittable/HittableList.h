#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

struct HittableList : public Hittable {
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {};
	HittableList(shared_ptr<Hittable> object) { add(object); };

	void clear();
	void add(shared_ptr<Hittable> object);
	virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
};

#endif // HITTABLELIST_H
