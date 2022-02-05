#ifndef BOX_H
#define BOX_H

#include <vector>

#include "Plane.h"
#include "hittable.h"

class Box : public std::enable_shared_from_this<Box>, public Hittable {
public:
  Point3 cback, cfront;
  shared_ptr<Material> mat_ptr;

  Box(Point3 cb, Point3 cf, const shared_ptr<Material>& m);
  Box(const Box &o): cback(o.cback), cfront(o.cfront), mat_ptr(o.mat_ptr), box(o.box), faces(o.faces) {}

  Box operator+(const Box &o) const;
  Box& operator=(Box &o) {
    cback = o.cback;
    cfront = o.cfront;
    mat_ptr = o.mat_ptr;

    return *this;
  }

  bool is_inside(const Point3 &p) const;

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const override;

  virtual shared_ptr<const Box> bounding_box() const override;
  
  virtual void scale(const Vec3 &s) override;
  virtual void move(const Vec3 &o) override;

private:
  std::vector<Plane> faces;
  shared_ptr<Box> box;
};

#endif // BOX_H
