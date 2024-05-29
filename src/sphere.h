#pragma once
#include <iostream>
#include <vector>
#include "glm/glm.hpp"

std::vector<float> fibonacchi_sphere(int n_points) {
	std::vector<float> sphere(n_points * 3, 0.0f);

	float phi = (1 + sqrt(5)) / 2;
	for (int i = 0; i < n_points; i++) {
		float x = (float)i / phi;
		float y = (float)i / n_points;

		float theta = 2 * glm::pi<float>() * x;
		float uagh = acos(1 - 2 * y);
		
		sphere[3 * i] = cos(theta) * sin(uagh);
		sphere[3 * i + 1] = sin(theta) * sin(uagh);
		sphere[3 * i + 2] = cos(uagh);
	}

	return sphere;
}

/*
std::vector<float> sampleSphere(int n_points) {
	
	std::vector<float> sphere(n_points * 3, 0.0f);
	for (int i = 0; i < n_points; i++) {
		sphere[i] = 0.0f;
		sphere[i + 1] = 0.0f;
		sphere[i + 2] = 
	}
}
*/
