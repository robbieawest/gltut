#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "helper.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "mouse.h"
#include "sphere.h"
#include "light.h"
#include "ResourceManager.h"
#include "RenderInstance.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <random>


//Camera and mouse
Camera cam(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
Mouse mouse(0.9f, cam);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	cam.key_callback(window, key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mouse.mouse_callback(window, xpos, ypos);
}


void mainLoop() {
	assert_cond(window, "window setup check");

	/*
	TODO:
	Shader is to be decoupled from RenderInstance
	RenderInstance methods to share gpu memory should be implemented
	Light class needs a fucking rework (hold a RenderInstance, get rid of the stupid static functionality)
	Implement texture parameter storage in RenderInstances, apply them on render().
	*/
	
	//New
	RenderInstance cube;
	cube.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	Shader cubeShader = Shader::parse("vertex", "fragment");
	cubeShader.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");
	
	RenderInstance lightInstance1;
	lightInstance1.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	Shader lightShader1 = Shader::parse("lightVertex", "lightFragment");
	lightShader1.set_mvpn("model_mat", "view_mat", "project_mat", "");

	RenderInstance lightInstance2;
	lightInstance2.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	Shader lightShader2 = Shader::parse("lightVertex", "lightFragment");
	lightShader2.set_mvpn("model_mat", "view_mat", "project_mat", "");

	RenderInstance floor;
	floor.readData("floorVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	Shader floorShader = Shader::parse("vertex", "fragment");
	floorShader.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");

	//MVP for object
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cam.pos, cam.pos + cam.dir, cam.up);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	cubeShader.attach();
	cubeShader.model(model, true);
	cubeShader.view(view);
	cubeShader.project(projection);
	cubeShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
	cubeShader.set_mat_uniforms("material.specular","material.shininess");
	cubeShader.set_material(
		32.0f
	);

	glm::mat4 model_floor = glm::mat4(1.0f);
	model_floor = glm::translate(model_floor, glm::vec3(0.0f, -2.0f, 0.0f));
	model_floor = glm::scale(model_floor, glm::vec3(100.0f));
	floorShader.attach();
	floorShader.model(model_floor, true);
	floorShader.view(view);
	floorShader.project(projection);
	floorShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
	floorShader.set_mat_uniforms("material.specular","material.shininess");
	floorShader.set_material(
		18.0f
	);
	
	//Lights
	std::vector<Light> lights = {
		{

			glm::vec3(1.2f, 1.0f, 1.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f),
			1.0f,
			0.09f,
			0.032f,
			&lightShader1,
			[](float time) -> glm::vec3 {
				return glm::vec3(
					3.0f * cos(glfwGetTime()),
					glm::abs(5.0f * cos(glfwGetTime() / 3.0f)),
					3.0f * sin(glfwGetTime())
				);
			},
			"cubeVertices.csv",
			Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2})
		},

		{
			glm::vec3(1.2f, 1.0f, 1.0f),
			glm::vec3(0.4f, 0.1f, 0.1f),
			glm::vec3(0.8f, 0.1f, 0.1f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			1.0f,
			0.3f,
			0.2f,
			&lightShader2,
			[](float time) -> glm::vec3 {
				return glm::vec3(
					3.0f * cos(glfwGetTime() * 2.0f),
					glm::abs(5.0f * cos(glfwGetTime() / 8.0f)),
					3.0f * sin(glfwGetTime() * 2.0f)
				);
			},
			"cubeVertices.csv",
			Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2})
		}
	};

	Light::updateShader(cubeShader, lights);
	Light::updateShader(floorShader, lights);

	glm::mat4 modelLight = glm::mat4(1.0f);
	for (Light& light : lights) {
		light.updateFragmentUniforms();
		Shader* sh = light.getShader();
		sh->attach();
		sh->view(view);
		sh->project(projection);
		sh->model(modelLight, false);
	}

	cube.loadTexture("container2.png", "material.diffuse", cubeShader);
	cube.loadTexture("container2_specular.png", "material.specular", cubeShader);

	floor.loadTexture("container2.png", "material.diffuse", floorShader);
	floor.loadTexture("container2_specular.png", "material.specular", floorShader);

	size_t n_Cubes = 20;
	std::vector<glm::vec3> cubePositions(n_Cubes, glm::vec3(0.0f));
	std::vector<glm::vec3> cubeAxisOfRotations(n_Cubes, glm::vec3(0.0f));
	std::vector<float> cubeRotations(n_Cubes, 0.0f);
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution xz(-3.0f, 3.0f);
	std::uniform_real_distribution y(1.0f, 5.0f);
	std::uniform_real_distribution rot(0.0f, 2 * glm::pi<float>());
	std::uniform_real_distribution axis(0.0f, 1.0f);
	for (size_t i = 0; i < n_Cubes; i++) {
		cubePositions[i] = glm::vec3(xz(rd), y(rd), xz(rd));
		cubeAxisOfRotations[i] = glm::vec3(axis(rd), axis(rd), axis(rd));
		cubeRotations[i] = rot(rd);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glc(glEnable(GL_DEPTH_TEST));
	glc(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	

	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	//game loop
	while (!glfwWindowShouldClose(window)) {
		
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		cam.handle_movement(deltaTime);

		glc(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		view = glm::lookAt(cam.pos, cam.pos + cam.dir, cam.up);
		
		//Draw lights
		for (Light& light : lights) {
			light.updatePosition(currentTime);
			light.render(view);
		}
		
		cubeShader.attach();
		cubeShader.view(view);
		cubeShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
		Light::updateShaderOnlyPositions(cubeShader, lights);
		for (size_t i = 0; i < n_Cubes; i++) {
			glm::mat4 mod = glm::mat4(1.0f);
			mod = glm::translate(mod, cubePositions[i]);
			mod = glm::rotate(mod, cubeRotations[i], cubeAxisOfRotations[i]);
			cubeShader.model(mod, true);
			cube.render();
		}
	
		floorShader.attach();
		floorShader.view(view);
		floorShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
		Light::updateShaderOnlyPositions(floorShader, lights);
		floor.render();
		
		//Unbind
		glc(glBindVertexArray(0));
		floorShader.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
