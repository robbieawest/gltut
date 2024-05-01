#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"
#include "GLError.h"

class Camera {
private:

	bool w_key_pressed = false;
	bool s_key_pressed = false;
	bool a_key_pressed = false;
	bool d_key_pressed = false;
	bool space_pressed = false;
	bool cntrl_pressed = false;
	
public:
	glm::vec3 pos, dir, up;
	float speed;
	Camera(glm::vec3 startPos, glm::vec3 startDir, glm::vec3 up, float speed) : pos(startPos), dir(startDir), up(up), speed(speed) {
	}


	void handle_movement(float deltaTime) {
		if (w_key_pressed) {
			pos += dir * speed * deltaTime;
		}
		if (s_key_pressed) {
			pos -= dir * speed * deltaTime;
		}
		if (a_key_pressed) {
			pos -= glm::normalize(glm::cross(dir, up)) * speed * deltaTime;
		}
		if (d_key_pressed) {
			pos += glm::normalize(glm::cross(dir, up)) * speed * deltaTime;
		}
		if (space_pressed) {
			pos += glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
		}
		if (cntrl_pressed) {
			pos -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
		}
	}

	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			spdlog::info("ESC pressed, exiting window");
			glfwSetWindowShouldClose(window, true);
		}
		if (key == GLFW_KEY_F && action == GLFW_PRESS) {
			spdlog::info("F key pressed, wireframes activated.");

			int currentMode;
			glc(glGetIntegerv(GL_POLYGON_MODE, &currentMode));

			int newMode = currentMode == GL_FILL ? GL_LINE : GL_FILL;
			glc(glPolygonMode(GL_FRONT_AND_BACK, newMode));
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			a_key_pressed = true;
		}
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
			a_key_pressed = false;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			s_key_pressed = true;
		}
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
			s_key_pressed = false;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			d_key_pressed = true;
		}
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
			d_key_pressed = false;
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			w_key_pressed = true;
		}
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
			w_key_pressed = false;
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			space_pressed = true;
		}
		else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
			space_pressed = false;
		}
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
			cntrl_pressed = true;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
			cntrl_pressed = false;
		}
	}
};
