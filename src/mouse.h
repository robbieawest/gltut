#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "camera.h"

class Mouse {
private:
	double last_mouse_xpos = 0.0f;
	double last_mouse_ypos = 0.0f;
	bool first_mouse_movement = true;
	const float mouse_sensitivity = 0.9f;
	float mouse_yaw = -90.0f, mouse_pitch = 0.0f;

	Camera& camera;

public:

	Mouse(float sensi, Camera& cam) : mouse_sensitivity(sensi), camera(cam) {}

	void mouse_callback(GLFWwindow* _, double xpos, double ypos) {
		if (first_mouse_movement) {
			last_mouse_xpos = xpos;
			last_mouse_ypos = ypos;
			first_mouse_movement = false;
			return;
		}

		double dx = xpos - last_mouse_xpos;
		double dy = last_mouse_ypos - ypos;
		last_mouse_xpos = xpos;
		last_mouse_ypos = ypos;

		mouse_yaw += mouse_sensitivity * 0.1 *  dx;
		mouse_pitch += mouse_sensitivity * 0.1 * dy;

		if (mouse_pitch > 89.0f)
			mouse_pitch = 89.0f;
		else if (mouse_pitch < -89.0f)
			mouse_pitch = -89.0f;

		glm::vec3 direction(
			cos(glm::radians(mouse_yaw)) * cos(glm::radians(mouse_pitch)),
			sin(glm::radians(mouse_pitch)),
			sin(glm::radians(mouse_yaw)) * cos(glm::radians(mouse_pitch))
		);

		camera.dir = glm::normalize(direction);
	}
};
