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



class Dielectric : public Material {
public:
	Dielectric(double ir): ir(ir) {}
	virtual bool scatter(
		const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered
	) const override {
		attenuation = Color(1, 1, 1);
		double ref_ratio = rec.front_face ? 1/ir : ir;

		Vec3 unit_dir = r_in.dir.normalized();
		double cos_theta = fmin(dot(-unit_dir, rec.normal), 1);
		double sin_theta = sqrt(1 - cos_theta*cos_theta);
		bool cannot_refract = ref_ratio * sin_theta > 1;
		Vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ref_ratio) > random_double())
			direction = reflect(unit_dir, rec.normal);
		else
			direction = refract(unit_dir, rec.normal, ref_ratio);

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1-ref_idx) / (1+ref_idx);
		r0 = r0*r0;
		return r0 + (1-r0)*pow((1 - cosine),5);
	}
	double ir;
};
