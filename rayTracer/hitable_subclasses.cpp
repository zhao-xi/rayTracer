#include"hitable.h"

/* ============================= sphere ==================================== */
bool sphere::hit(const Ray& r, float tmin, float tmax, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (2 * a);
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / (2 * a);
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
	}
	return false;
}
/* ========================== moving_sphere ================================ */
vec3 moving_sphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}
bool moving_sphere::hit(const Ray& r, float tmin, float tmax, hit_record &rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (2 * a);
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time()))/radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / (2 * a);
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}