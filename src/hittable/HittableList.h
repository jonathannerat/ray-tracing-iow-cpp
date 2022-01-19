#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

struct HittableList : public Hittable {
  std::vector<shared_ptr<Hittable>> objects;

  HittableList(){};
  HittableList(shared_ptr<Hittable> object) { add(object); };

  void clear();
  void add(shared_ptr<Hittable> object);
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual shared_ptr<const Box> bounding_box() const override;

private:
  shared_ptr<const Box> box;
};

#endif // HITTABLELIST_H
