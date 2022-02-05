#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class Triangle : public Hittable {
public:
  Point3 p0, p1, p2;

  shared_ptr<Material> mat_ptr;

  Triangle(Point3 p0, Point3 p1, Point3 p2, shared_ptr<Material> m);

  void scale_inplace(const Vec3 &s);

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;
  virtual shared_ptr<const Box> bounding_box() const override;

  virtual void scale(const Vec3 &s) override;
  virtual void move(const Vec3 &o) override;
private:
  std::shared_ptr<const Box> box;
};

#endif // TRIANGLE_H
