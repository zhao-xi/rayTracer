#include <fstream>
#include <cfloat>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "hitableList.h"
using namespace std;

vec3 color(Ray& r,hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	ofstream file;
	file.open("image.ppm", ios::out);
	int nx = 800;
	int ny = 400;
	file << "P3" << endl;
	file << nx << " " << ny << endl << 255 << endl;

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	hitable *list[2];
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5);
	list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100);
	hitable *world = new hitable_list(list, 2);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r, world);
			float ir = int(col[0] * 255.99);
			float ig = int(col[1] * 255.99);
			float ib = int(col[2] * 255.99);
			file << ir << " " << ig << " " << ib << endl;
		}
	}
	return 0;
}