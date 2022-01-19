#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
  Point3 center;
  double radius;
  shared_ptr<Material> mat_ptr;

  Sphere(Point3 c, double r, shared_ptr<Material> m);

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual std::shared_ptr<const Box> bounding_box() const override;

private:
  shared_ptr<Box> box;
};

#endif // SPHERE_H
