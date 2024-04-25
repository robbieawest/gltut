#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include "spdlog/spdlog.h"
#include "GLError.h"


//Constants
unsigned int SCREEN_WIDTH = 1920;
unsigned int SCREEN_HEIGHT = 1080;

//GLFW and GLEW setup
GLFWwindow* window;

//Window resize callback updates gl viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glc(glViewport(0, 0, width, height));
}

void init() {

	if (!glfwInit())
		throw std::invalid_argument("glfw could not be initialized.");


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported)glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	if (glewInit() != GLEW_OK)
		throw std::invalid_argument("Err while initializing GLEW.");
	
	spdlog::info("glfw and glew initialized successfully.");
}

/*
//Load model vertices from file
void load_vertices(std::string filename, int n_properties, int indices[], float vertices[]) {
	if (sizeof(vertices) / sizeof(float) != n_properties) {
		spdlog::error("vertices size given is not consistent with the number of properties.");
		return;
	}
	
	std::vector<std::string> spl_res = split(filename, '.');
	if (spl_res.size() != 2)spdlog::error("Invalid filename format: {}", filename);
	std::string extension = spl_res[1];
	if (extension != "csv")spdlog::error("Invalid file extension: {}", extension);

	std::string path = format_g("./Resources/Models/{}", filename);

	std::ifstream file_stream(path);
	if (!file_stream.is_open()) {
		spdlog::error("Could not open file {}", path);
		return;
	}
	file_stream.ignore('\n');//Ignore first line

	std::string curr_line;
	int i = 0;
	while (std::getline(file_stream, curr_line)) {

		std::vector<std::string> split_line = split(curr_line, ',');
		
		
		vertices[i] = 
		i++;
	}
}
*/



//To be called on exit of program
int exit() {
	glfwTerminate();
	return 0;
}




