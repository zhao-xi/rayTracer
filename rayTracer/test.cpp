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

int main() {
	ofstream file;
	file.open("image.ppm", ios::out);
	int nx = 800;
	int ny = 400;
	int ns = 100;
	file << "P3" << endl;
	file << nx << " " << ny << endl << 255 << endl;
	
	hitable *list[5];
	list[0] = new sphere(vec3(-0.0, 0.0, -1.0), 0.5,new lambertian(vec3(0.0,0.0,1.0)));
	list[1] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
	list[2] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new metal(vec3(0.2, 0.2, 0.5), 0.3));
	list[3] = new sphere(vec3(1.0, 0.0, -1.0), -0.45, new dielectric(1.5)); // to make the dielectric sphere hollow
	list[4] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.2, 0.5, 0.3)));

	hitable *world = new hitable_list(list, 5);

	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	Camera cam(lookfrom, lookat, vec3(0, 1, 0), 45.0, float(nx) / float(ny), 2.0, (lookfrom - lookat).length());
	//int count = 0;
	for (int j = ny - 1; j >= 0; j--) {
		cout << "Progress: " << 100 - j / 4 << "%\r";
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + float(rand())/RAND_MAX) / float(nx);
				float v = float(j + float(rand())/RAND_MAX) / float(ny);
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