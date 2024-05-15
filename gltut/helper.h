#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "glm/glm.hpp"
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

typedef std::pair<std::vector<float>, std::vector<unsigned int>> vertex_data;

//Load model vertices from file
vertex_data load_vertex_data(std::string filename) {
	vertex_data out = std::make_pair(std::vector<float>(), std::vector<unsigned int>());
	
	std::vector<std::string> spl_res = split(filename, '.');
	if (spl_res.size() != 2)spdlog::error("Invalid filename format: {}", filename);
	std::string extension = spl_res[1];
	if (extension != "csv")spdlog::error("Invalid file extension: {}", extension);

	std::string path = format_g("./Resources/Models/{}", filename);

	std::ifstream file_stream(path);
	if (!file_stream.is_open()) {
		spdlog::error("Could not open file {}", path);
		return out;
	}

	std::string curr_line;
	int i = 0;
	bool readingVertexData = true;
	while (std::getline(file_stream, curr_line)) {
		if (i == 0) {
			i++;
			continue;
		}

		if (curr_line.empty())continue;
		if (curr_line[0] == '/') {
			readingVertexData = false;
			continue;
		}

		std::vector<std::string> split_line = split(curr_line, ',');
		for (std::string& data : split_line) {
			try {
				if (readingVertexData)
					out.first.push_back(std::stof(data));
				else
					out.second.push_back(std::stoul(data));
			}
			catch (const std::exception& ex) {
				spdlog::error("Error while parsing vertex data on line {}", i + 1);
				spdlog::error(ex.what());
			}
		}
		
		i++;
	}

	file_stream.close();

	return out;
}



//To be called on exit of program
int exit() {
	glfwTerminate();
	return 0;
}




