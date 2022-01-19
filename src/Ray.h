#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

struct Ray {
  Point3 origin;
  Vec3 dir;

  Ray() {}
  Ray(const Point3 &o, const Vec3 &d) : origin(o), dir(d) {}

  Point3 operator()(double t) const { return this->origin + t * this->dir; }
};

#endif // RAY_H
