#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"

class Plane : public Hittable {
public:
  Point3 origin;
  Vec3 normal;
  shared_ptr<Material> mat_ptr;

  Plane(Point3 o, Vec3 n, shared_ptr<Material> m)
      : origin(o), normal(n), mat_ptr(m) { refp = o; };
  Plane(const Plane &o): origin(o.origin), normal(o.normal), mat_ptr(o.mat_ptr) {};

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual shared_ptr<const Box> bounding_box() const override;

  virtual void scale(const Vec3 &s) override;
  virtual void move(const Vec3 &o) override;
};

#endif // PLANE_H
