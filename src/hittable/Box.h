#ifndef BOX_H
#define BOX_H

#include <vector>

#include "Plane.h"
#include "hittable.h"

class Box : public std::enable_shared_from_this<Box>, public Hittable {
public:
  Point3 cback, cfront;
  shared_ptr<Material> mat_ptr;

  Box(Point3 cb, Point3 cf, shared_ptr<Material> m);

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual shared_ptr<const Box> bounding_box() const override;

  bool is_inside(const Point3 &p) const;

private:
  std::vector<Plane> faces;
  shared_ptr<Box> box;
};

#endif // BOX_H
