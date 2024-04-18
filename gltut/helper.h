#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include "spdlog/spdlog.h"
#include "GLError.h"


//Constants
unsigned int SCREEN_WIDTH = 900;
unsigned int SCREEN_HEIGHT = 900;

//GLFW and GLEW setup
GLFWwindow* window;

//Window resize callback updates gl viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glc(glViewport(0, 0, width, height));
}

void init() {

	if (!glfwInit())
		throw std::invalid_argument("glfw could not be initialized.");


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gltut", NULL, NULL);
	if (!window) {
		glfwTerminate();
		spdlog::error("glfw window could not be initialized.");
		throw std::invalid_argument("glfw window could not be initialized.");
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glc(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (glewInit() != GLEW_OK)
		throw std::invalid_argument("Err while initializing GLEW.");
	
	
	spdlog::info("glfw and glew initialized successfully.");
}


//For all inputs
void processInputs() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		spdlog::info("ESC pressed, exiting window");
		glfwSetWindowShouldClose(window, true);
	}
}


//To be called on exit of program
int exit() {
	glfwTerminate();
	return 0;
}




