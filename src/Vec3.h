#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

#include <cmath>
#include <iostream>

struct Vec3 {
	inline static Vec3 random() {
		return Vec3(random_double(), random_double(), random_double());
	}

	inline static Vec3 random(double min, double max) {
		return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
	}

	double x, y, z;

	Vec3(): x(0), y(0), z(0) {}
	Vec3(double x, double y, double z): x(x), y(y), z(z) {}

	Vec3 operator-() const;
	Vec3& operator+=(const Vec3 &v);
	Vec3& operator*=(double t);
	Vec3& operator/=(double t);

	inline Vec3 operator+(const Vec3 &b) const {
		return Vec3(this->x + b.x, this->y + b.y, this->z + b.z);
	}

	inline Vec3 operator-(const Vec3 &b) const {
		return Vec3(this->x - b.x, this->y - b.y, this->z - b.z);
	}

	inline Vec3 operator*(const Vec3 &b) const {
		return Vec3(this->x * b.x, this->y * b.y, this->z * b.z);
	}

	inline Vec3 operator*(double t) const {
		return Vec3(this->x * t, this->y * t, this->z * t);
	}

	inline Vec3 operator/(double t) const { return *this * (1/t); }

	inline Vec3 normalized() const {
		return *this / this->length();
	}

	bool near_zero() const;
	double length() const;
	double length2() const;
};

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
	return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator*(double t, const Vec3 &v) { return v * t; }

inline double dot(const Vec3 &a, const Vec3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
	return Vec3(a.y * b.z - b.y * a.z,
	            b.x * a.z - a.x * b.z,
	            a.x * b.y - b.x * a.y);
}

inline Vec3 random_in_unit_sphere() {
	while (true) {
		Vec3 p = Vec3::random(-1, 1);
		if (p.length2() >= 1) continue;
		return p;
	}
}

inline Vec3 random_unit_vector() {
	return random_in_unit_sphere().normalized();
}

inline Vec3 random_in_hemisphere(const Vec3 &normal) {
	Vec3 in_unit_sphere = random_in_unit_sphere();
	return dot(in_unit_sphere, normal) > 0 ? in_unit_sphere : -in_unit_sphere;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
	return v - 2*dot(v, n)*n;
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat) {
	double cos_theta = fmin(dot(-uv, n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_par = - sqrt(fabs(1.0 - r_out_perp.length2())) * n;
	return r_out_perp + r_out_par;
}

using Point3 = Vec3;
using Color = Vec3;

#endif // VEC3_H
