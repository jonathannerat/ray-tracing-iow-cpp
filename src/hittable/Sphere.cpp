#include "Sphere.h"
#include "Box.h"

using std::sqrt;

Sphere::Sphere(Point3 c, double r, shared_ptr<Material> m)
    : center(c), radius(r), mat_ptr(m) {
  box = make_shared<Box>(c - Vec3(r, r, r), c + Vec3(r, r, r), nullptr);
  refp = c;
}
bool Sphere::hit(const Ray &r, double t_min, double t_max,
                 HitRecord &rec) const {
  Vec3 oc = r.origin - center;
  double a = r.dir.length2();
  double hb = dot(oc, r.dir);
  double c = oc.length2() - radius * radius;

  double discriminant = hb * hb - a * c;
  if (discriminant < 0)
    return false;

  double sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  double root = (-hb - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-hb + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r(rec.t);
  Vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  return true;
}

std::shared_ptr<const Box> Sphere::bounding_box() const { return box; }
