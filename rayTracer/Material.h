#pragma once
#include"ray.h"
#include"hitable.h"

class Material {
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const = 0;
};

/*functions gonna be used*/

//reflection
inline vec3 reflect(const vec3& v, const vec3& normal) {
	return v - 2 * dot(v, normal)*normal;
}
//refraction
inline bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);// (1/sin(theta))==(n1/n2)
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else return false;
}

/*subclasses of material*/
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

class dielectric :public Material {
public:
	dielectric(float ri) :ref_idx(ri) {}
	bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			scattered = Ray(rec.p, refracted);
		}
		else {
			scattered = Ray(rec.p, reflected);
			return false;
		}
		return true;
	}

	float ref_idx;
};