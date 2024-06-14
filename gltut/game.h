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
	
	//New
	RenderInstance cube("vertex", "fragment");
	cube.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	
	RenderInstance lightInstance1("lightVertex", "lightFragment");
	lightInstance1.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));

	RenderInstance lightInstance2("lightVertex", "lightFragment");
	lightInstance2.readData("cubeVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));

	RenderInstance floor("vertex", "fragment");
	floor.readData("floorVertices.csv", Resource::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));
	
	Shader& lightShader1 = lightInstance1.getShader();
	Shader& lightShader2 = lightInstance2.getShader();

	lightShader1.set_mvpn("model_mat", "view_mat", "project_mat", "");
	lightShader2.set_mvpn("model_mat", "view_mat", "project_mat", "");

	Shader& cubeShader = cube.getShader();
	Shader& floorShader = floor.getShader();

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
			}
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
			}
		}
	};

	Light::updateShader(cubeShader, lights);
	Light::updateShader(floorShader, lights);
	Light::updateLightFragmentUniforms(lights);

	glm::mat4 modelLight = glm::mat4(1.0f);

	floorShader.attach();
	modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, lights[0].position);
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));
	
	for (Light& light : lights) {
		light.shaderObj->attach();
		light.shaderObj->view(view);
		light.shaderObj->project(projection);
		light.shaderObj->model(modelLight, false);
	}

	cube.loadTexture("container2.png", "material.diffuse");
	cube.loadTexture("container2_specular.png", "material.specular");

	floorShader.attach();
	Texture floor_diffuse = Texture::from_image("container2.png", floorShader, "material.diffuse");
	Texture floor_specular = Texture::from_image("container2_specular.png", floorShader, "material.specular");


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

		//rendering
		glc(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Update light position
		Light::updateLightsPositions(lights, currentTime);
		Light::updateShaderOnlyPositions(cubeShader, lights);
		Light::updateShaderOnlyPositions(floorShader, lights);

		view = glm::lookAt(cam.pos, cam.pos + cam.dir, cam.up);
		
		//Update and draw object
		
		cubeShader.attach();
		cubeShader.view(view);
		cubeShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

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
		floor.render();

		//Draw lights
		lightShader1.attach();
		lightShader1.view(view);
		modelLight = glm::mat4(1.0f);
		modelLight = glm::translate(modelLight, lights[0].position);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		lightShader1.model(modelLight, false);
		lightInstance1.render();

		lightShader2.attach();
		lightShader2.view(view);
		modelLight = glm::mat4(1.0f);
		modelLight = glm::translate(modelLight, lights[1].position);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		lightShader2.model(modelLight, false);
		lightInstance2.render();
		
		//Unbind
		glc(glBindVertexArray(0));
		floorShader.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
