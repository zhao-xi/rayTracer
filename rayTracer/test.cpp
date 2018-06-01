#include <fstream>
#include <cfloat>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitableList.h"
#include "camera.h"
#include "Material.h"
using namespace std;

vec3 color(const Ray& r, hitable *world,int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth + 1);
		}
		else { 
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_1();
			vec3 center(a + 0.9*random_1(),0.2,b+0.9*random_1());
			if (((center - vec3(4, 0.2, 0)).length() > 0.9)&&((center - vec3(-4, 0.2, 0)).length() > 0.9)&&((center - vec3(0, 0.2, 0)).length() > 0.9)) {
				if (choose_mat < 0.8) { // diffuse
					list[i++] = new moving_sphere(center, center + vec3(0, 0.5*random_1(), 0), 0.0, 1.0, 0.2, new lambertian(vec3(random_1()*random_1(), random_1()*random_1(), random_1()*random_1())));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2, 
						new metal(vec3(0.5*(1 + random_1()), 0.5*(1 + random_1()), 0.5*(1 + random_1())), 0.5*random_1()));
				}
				else { // dieletric
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.6, 0.5, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.2, 0.6, 0.5), 0.0));
	list[i++] = new sphere(vec3(0, 1, 0), -0.95, new dielectric(1.5));
	list[i++] = new sphere(vec3(-8, 1, 0), 1.0, new dielectric(1.5));
	return new hitable_list(list, i);
}

int main() {
	ofstream file;
	file.open("image.ppm", ios::out);
	int nx = 400;
	int ny = 200;
	int ns = 10;
	file << "P3" << endl;
	file << nx << " " << ny << endl << 255 << endl;
	


	hitable *world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float focus_dist = 10.0;
	Camera cam(lookfrom, lookat, vec3(0, 1, 0), 20.0, float(nx) / float(ny), 0.0, focus_dist, 0.0, 1.0);
	//int count = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_1()) / float(nx);
				float v = float(j + random_1()) / float(ny);
				Ray r = cam.get_ray(u, v);
			//	vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			float ir = int(col[0] * 255.99);
			float ig = int(col[1] * 255.99);
			float ib = int(col[2] * 255.99);
			file << ir << " " << ig << " " << ib << endl;
		}
	}
	return 0;
}