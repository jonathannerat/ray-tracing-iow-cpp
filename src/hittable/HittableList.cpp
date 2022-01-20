#include <iostream>

#include "Box.h"
#include "HittableList.h"

using namespace std;

void HittableList::clear() {
  objects.clear();
  refpacc = Point3();
}

void HittableList::add(shared_ptr<Hittable> object) {
  shared_ptr<const Box> objbox = object->bounding_box();

  objects.push_back(object);

  refpacc += object->refp;
  refp = refpacc / objects.size();

  if (!box)
    box = objbox;
  else if (objbox)
    box = make_shared<Box>(*box + *objbox);
}

bool HittableList::hit(const Ray &r, double t_min, double t_max,
                       HitRecord &rec) const {
  HitRecord tmp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (auto object : objects) {
    if (object->hit(r, t_min, closest_so_far, tmp_rec)) {
      hit_anything = true;
      closest_so_far = tmp_rec.t;
      rec = tmp_rec;
    }
  }

  return hit_anything;
}

shared_ptr<const Box> HittableList::bounding_box() const { return box; }

HittableList &HittableList::operator=(const HittableList &o) {
  box = o.box;
  objects = o.objects;
  return *this;
}
