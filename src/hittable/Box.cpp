#include <iostream>

#include "Box.h"

using namespace std;

Box::Box(Point3 c1, Point3 c2, shared_ptr<Material> m)
    : corner1(c1), corner2(c2), mat_ptr(m) {
  Vec3 x(c1.x < c2.x ? 1 : -1, 0, 0);
  Vec3 y(0, c1.y < c2.y ? 1 : -1, 0);
  Vec3 z(0, 0, c1.z < c2.z ? 1 : -1);

  faces.push_back(Plane(c1, -x, m));
  faces.push_back(Plane(c1, y, m));
  faces.push_back(Plane(c1, z, m));

  faces.push_back(Plane(c2, x, m));
  faces.push_back(Plane(c2, -y, m));
  faces.push_back(Plane(c2, -z, m));
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

  inside &= corner1.x < corner2.x
                ? (corner1.x - EPS <= p.x && p.x <= corner2.x + EPS)
                : (corner2.x - EPS <= p.x && p.x <= corner1.x + EPS);
  inside &= corner1.y < corner2.y
                ? (corner1.y - EPS <= p.y && p.y <= corner2.y + EPS)
                : (corner2.y - EPS <= p.y && p.y <= corner1.y + EPS);
  inside &= corner1.z < corner2.z
                ? (corner1.z - EPS <= p.z && p.z <= corner2.z + EPS)
                : (corner2.z - EPS <= p.z && p.z <= corner1.z + EPS);

  return inside;
}
