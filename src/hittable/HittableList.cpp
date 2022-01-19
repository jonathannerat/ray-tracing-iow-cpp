#include <iostream>

#include "Box.h"
#include "HittableList.h"

using namespace std;

void HittableList::clear() { objects.clear(); }

void HittableList::add(shared_ptr<Hittable> object) {
  shared_ptr<const Box> objbox = object->bounding_box();

  objects.push_back(object);

  if (!box) {
    box = objbox;
  } else if (objbox) {
    Point3 cback = box->cback, cfront = box->cfront;
    if (box->is_inside(objbox->cback))
      cback = Point3(MIN(box->cback.x, objbox->cback.x),
                     MIN(box->cback.y, objbox->cback.y),
                     MIN(box->cback.z, objbox->cback.z));

    if (box->is_inside(objbox->cfront))
      cfront = Point3(MAX(box->cfront.x, objbox->cfront.x),
                      MAX(box->cfront.y, objbox->cfront.y),
                      MAX(box->cfront.z, objbox->cfront.z));

    if (box->cback != cback && box->cfront != cfront)
      box = make_shared<Box>(cback, cfront, nullptr);
  }
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
