#pragma once

#include "../hittable/hittable.h"

class Material {
	public:
		virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};



class Lambertian : public Material {
public:
	Lambertian(const Color &a): albedo(a) {}

	virtual bool scatter(
		const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
	) const override {
		Vec3 scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

	Color albedo;
};



class Metal : public Material {
public:
	Metal(const Color &a, double f) : albedo(a), fuzz(f) {}

	virtual bool scatter(
		const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
	) const override {
		Vec3 reflected = reflect(r_in.dir.normalized(), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return dot(reflected, rec.normal) > 0;
	}

	Color albedo;
	double fuzz;
};
