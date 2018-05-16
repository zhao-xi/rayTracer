#pragma once
#include"ray.h"
#include"hitable.h"

class Material {
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const = 0;
};

class lambertian :public Material {
public:
	lambertian(const vec3& a) :albedo(a) {}
	bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

inline vec3 reflect(const vec3& v, const vec3& normal) {
	return v - 2 * dot(v, normal)*normal;
}
class metal :public Material {
public:
	metal(const vec3& a, float f) :albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return true;
	}
	vec3 albedo;
	float fuzz;
};