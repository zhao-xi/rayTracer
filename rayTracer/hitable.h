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

/*funcitons gonna be used*/
inline vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}


/*subclasses*/
class sphere :public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, Material *ptr) :center(cen), radius(r), mat(ptr) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	Material *mat;
};


class moving_sphere :public hitable {
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, Material *m) :center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center(float time) const;
	vec3 center0, center1;
	float time0, time1;
	float radius;
	Material *mat_ptr;
};