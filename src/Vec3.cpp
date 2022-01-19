#include "Vec3.h"

Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }

Vec3 &Vec3::operator+=(const Vec3 &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vec3 &Vec3::operator*=(double t) {
  x *= t;
  y *= t;
  z *= t;
  return *this;
}

Vec3 &Vec3::operator/=(double t) { return *this *= 1 / t; }

bool Vec3::near_zero() const {
  return (fabs(x) < EPS) && (fabs(y) < EPS) && (fabs(z) < EPS);
}

double Vec3::length() const { return sqrt(this->length2()); }

double Vec3::length2() const { return x * x + y * y + z * z; }

bool Vec3::operator==(const Vec3 &o) const {
  return this->x == o.x && this->y == o.y && this->z == o.z;
}

bool Vec3::operator!=(const Vec3 &o) const {
  return !(*this == o);
}
