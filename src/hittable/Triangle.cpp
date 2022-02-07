#include "Triangle.h"
#include "Box.h"

Triangle::Triangle(Point3 p0, Point3 p1, Point3 p2, shared_ptr<Material> m)
    : p0(p0), p1(p1), p2(p2), mat_ptr(m) {
  Point3 cback(
    MIN(MIN(p0.x, p1.x), p2.x),
    MIN(MIN(p0.y, p1.y), p2.y),
    MIN(MIN(p0.z, p1.z), p2.z));
  Point3 cfront(
    MAX(MAX(p0.x, p1.x), p2.x),
    MAX(MAX(p0.y, p1.y), p2.y),
    MAX(MAX(p0.z, p1.z), p2.z));

  box = make_shared<const Box>(cback, cfront, nullptr);
  refp = (p0 + p1 + p2)/3;
}

void Triangle::scale_inplace(const Vec3 &s) {
    p0 = p0 * s;
    p1 = p1 * s;
    p2 = p2 * s;
    refp = refp * s;

    box = make_shared<const Box>(box->cback * s, box->cfront * s, box->mat_ptr);
}

bool Triangle::hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const {
  Vec3 normal = cross(p1 - p0, p2 - p0);

  if (perpendicular(normal, r.dir))
    return false;

  double t = dot(p0 - r.origin, normal) / dot(r.dir, normal);
  if (t < t_min || t_max < t)
    return false;

  Point3 p = r(t);

  if (dot(normal, cross(p1 - p0, p - p0)) > 0 &&
      dot(normal, cross(p2 - p1, p - p1)) > 0 &&
      dot(normal, cross(p0 - p2, p - p2)) > 0) {
    rec.t = t;
    rec.p = p;
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
    return true;
  } else {
    return false;
  }
}

shared_ptr<const Box> Triangle::bounding_box() const { return box; }

void Triangle::scale(const Vec3 &s) {
  p0 = (p0 - refp) * s + refp;
  p1 = (p1 - refp) * s + refp;
  p2 = (p2 - refp) * s + refp;

  auto newbox = make_shared<Box>(*box);
  newbox->scale(s);
  box = newbox;
}

void Triangle::move(const Vec3 &o) {
  p0 += o;
  p1 += o;
  p2 += o;

  auto newbox = make_shared<Box>(*box);
  newbox->move(o);
  box = newbox;
}
