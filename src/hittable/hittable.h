#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Ray.h"
#include "../Vec3.h"

class Material;
class Box;

struct HitRecord {
  Point3 p;
  Vec3 normal;
  double t;
  bool front_face;

  shared_ptr<Material> mat_ptr;

  inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
    front_face = dot(r.dir, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
public:
  Point3 refp;
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const = 0;

  virtual std::shared_ptr<const Box> bounding_box() const = 0;
  virtual void scale(const Vec3 &s) = 0;
  virtual void move(const Vec3 &o) = 0;
};

#endif // HITTABLE_H
