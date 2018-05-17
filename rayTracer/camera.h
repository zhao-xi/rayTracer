#pragma once
#define _USE_MATH_DEFINES
#include "ray.h"
#include<math.h>

class Camera {
public:
	Camera(float vfov, float aspect) { // vfov : top to bottom in degrees
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		lower_left_corner = vec3(-half_width, -half_height, -1.0);
		horizontal = vec3(2 * half_width, 0.0, 0.0);
		vertical = vec3(0.0, 2 * half_height, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}
	Ray get_ray(float u, float v) {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};