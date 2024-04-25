#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "helper.h"
#include "shader.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 5.0f;
float deltaTime = 0.0f;

bool w_key_pressed = false;
bool s_key_pressed = false;
bool a_key_pressed = false;
bool d_key_pressed = false;
bool space_pressed = false;
bool cntrl_pressed = false;

void handle_movement() {
	if (w_key_pressed) {
		cameraPos += cameraFront * cameraSpeed * deltaTime;
	}
	if (s_key_pressed) {
		cameraPos -= cameraFront * cameraSpeed * deltaTime;
	}
	if (a_key_pressed) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
	if (d_key_pressed) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
	if (space_pressed) {
		cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime;
	}
	if (cntrl_pressed) {
		cameraPos -= glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime;
	}
}

//For all inputs
void key_callback(GLFWwindow* _, int key, int scancode, int action, int mods) {

	
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

double last_mouse_xpos = 0.0f;
double last_mouse_ypos = 0.0f;
bool first_mouse_movement = true;
const float mouse_sensitivity = 0.9f;
float mouse_yaw = -90.0f, mouse_pitch = 0.0f;

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

	cameraFront = glm::normalize(direction);
}


void mainLoop() {
	assert_cond(window, "window setup check");

//	float vertices[] = {
//		0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //top right
//		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //bottom right
//		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  //bottom left
//		-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  //top left
//	};
	float verticesold[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	float vertices[8 * 3] = {
		0.5f, 0.5f, 0.5f, //back face (into screen)
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, -0.5f, //front face (out of screen)
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f
	};

	unsigned int indices[] = {
		0, 1, 2, //back face
		2, 3, 0,
		4, 5, 6, //front face
		6, 7, 4,
		0, 4, 3, //top face
		4, 3, 7,
		0, 4, 1, //right face
		4, 1, 5,
		7, 3, 2, //left face
		7, 6, 2
	};

	//Create vertex array object to bind our VBO and vertex linking to
	unsigned int VAO;
	glc(glGenVertexArrays(1, &VAO));
	glc(glBindVertexArray(VAO));

	//Object
	unsigned int VBO;
	glc(glGenBuffers(1, &VBO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	unsigned int EBO;
	glc(glGenBuffers(1, &EBO));
	glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	glc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));

	//Light source
	unsigned int LVAO;
	glc(glGenVertexArrays(1, &LVAO));
	glc(glBindVertexArray(LVAO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));



	//Shader
	Shader shaderProgram = Shader::parse("vertex", "fragment");
	shaderProgram.uniform3f("objectColour", 1.0f, 0.5f, -0.31f);
	shaderProgram.uniform3f("lightColour", 1.0f, 1.0f, 1.0f);

	Shader lightShader = Shader::parse("vertex", "lightFragment");

	//MVP for object
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	shaderProgram.attach();
	shaderProgram.uniformMatrix4f("m", model);
	shaderProgram.uniformMatrix4f("v", view);
	shaderProgram.uniformMatrix4f("p", projection);
	glm::mat4 modelLight = glm::mat4(1.0f);

	glm::vec3 light_position(1.2f, 1.0f, 1.0f);
	modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, light_position);
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));

	lightShader.attach();
	lightShader.uniformMatrix4f("m", modelLight);
	lightShader.uniformMatrix4f("v", view);
	lightShader.uniformMatrix4f("p", projection);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glc(glEnable(GL_DEPTH_TEST));


	float lastFrameTime = 0.0f;
	//game _loop_
	while (!glfwWindowShouldClose(window)) {
		
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		handle_movement();

		//rendering
		glc(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		shaderProgram.attach();

		//Update view matrix
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shaderProgram.uniformMatrix4f("v", view);
		
		//Draw object
		glc(glBindBuffer(GL_ARRAY_BUFFER, VAO));
		glc(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

		//Draw light
		
		lightShader.attach();
		lightShader.uniformMatrix4f("v", view);
		glc(glBindBuffer(GL_ARRAY_BUFFER, LVAO));
		glc(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
		

		//Unbind
		glc(glBindBuffer(GL_ARRAY_BUFFER, 0));
		shaderProgram.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
