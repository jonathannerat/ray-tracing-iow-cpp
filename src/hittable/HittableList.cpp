#include "HittableList.h"

void
HittableList::clear() {
	objects.clear();
}

void
HittableList::add(shared_ptr<Hittable> object) {
	objects.push_back(object);
}

bool
HittableList::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
	HitRecord tmp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;

	for (auto object: objects) {
		if (object->hit(r, t_min, closest_so_far, tmp_rec)) {
			hit_anything = true;
			closest_so_far = tmp_rec.t;
			rec = tmp_rec;
		}
	}

	return hit_anything;
}
