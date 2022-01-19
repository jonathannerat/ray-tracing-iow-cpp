#include <iostream>

#include "Box.h"

using namespace std;

Box::Box(Point3 cb, Point3 cf, shared_ptr<Material> m) : mat_ptr(m) {
  cback = Point3(MIN(cb.x, cf.x), MIN(cb.y, cf.y), MIN(cb.z, cf.z));
  cfront = Point3(MAX(cb.x, cf.x), MAX(cb.y, cf.y), MAX(cb.z, cf.z));

  Vec3 x(1, 0, 0), y(0, 1, 0), z(0, 0, 1);

  faces.push_back(Plane(cb, -x, m));
  faces.push_back(Plane(cb, -y, m));
  faces.push_back(Plane(cb, -z, m));

  faces.push_back(Plane(cf, x, m));
  faces.push_back(Plane(cf, y, m));
  faces.push_back(Plane(cf, z, m));
}

bool Box::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
  HitRecord tmp_rec;
  double closest_so_far = t_max;
  bool hit_anything = false;

  for (const auto &face : faces) {
    if (face.hit(r, t_min, closest_so_far, tmp_rec) && is_inside(tmp_rec.p)) {
      hit_anything = true;
      closest_so_far = tmp_rec.t;
      rec = tmp_rec;
    }
  }

  return hit_anything;
}

bool Box::is_inside(const Point3 &p) const {
  bool inside = true;

  inside &= cback.x < cfront.x
                ? (cback.x - EPS <= p.x && p.x <= cfront.x + EPS)
                : (cfront.x - EPS <= p.x && p.x <= cback.x + EPS);
  inside &= cback.y < cfront.y
                ? (cback.y - EPS <= p.y && p.y <= cfront.y + EPS)
                : (cfront.y - EPS <= p.y && p.y <= cback.y + EPS);
  inside &= cback.z < cfront.z
                ? (cback.z - EPS <= p.z && p.z <= cfront.z + EPS)
                : (cfront.z - EPS <= p.z && p.z <= cback.z + EPS);

  return inside;
}

shared_ptr<const Box> Box::bounding_box() const { return shared_from_this(); }
