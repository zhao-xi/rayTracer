#pragma once
#include "ray.h"

class Material;
struct hit_record {
	float t;
	vec3 p;
	vec3 normal;
	Material *mat_ptr;
};

class hitable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

inline vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

class sphere :public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, Material *ptr) :center(cen), radius(r), mat(ptr) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	Material *mat;
};
