#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>

#include "hittable.h"

using std::make_shared;
using std::shared_ptr;

struct HittableList : public Hittable {
  std::vector<shared_ptr<Hittable>> objects;

  HittableList(){};
  HittableList(const HittableList& o): box(o.box), objects(o.objects) {};
  HittableList(shared_ptr<Hittable> object) { add(object); };

  HittableList& operator=(const HittableList &o);

  void clear();
  size_t size() const { return objects.size(); };
  void add(shared_ptr<Hittable> object);
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual shared_ptr<const Box> bounding_box() const override;

private:
  shared_ptr<const Box> box;
  Point3 refpacc;
};

#endif // HITTABLELIST_H
