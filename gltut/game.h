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
#include "ObjectManager.h"

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


	vertex_data cubeData = load_vertex_data("cubeVertices.csv");
	std::vector<float> cubeVertices = cubeData.first;

	ObjectManager::ObjectManager* objectManager = ObjectManager::ObjectManager::getInstance();
	objId cubeObjectData = objectManager->registerObject();
	
	
	//Object
	objectManager->mapVertexArray(cubeObjectData);
	objectManager->mapVertexBuffer(cubeObjectData, cubeVertices.data(), cubeVertices.size());
	objectManager->expressVertexLayout(ObjectManager::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));

	//Light source
	objId lightData = objectManager->registerObject();
	objectManager->mapVertexArray(lightData);
	objectManager->mapSharedVertexBuffer(lightData, cubeObjectData);
	objectManager->expressVertexLayout(ObjectManager::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));


	//Floor
	vertex_data floor_data = load_vertex_data("floorVertices.csv");
	std::vector<float> floorVertices = floor_data.first;
	std::vector<unsigned int> floorIndices = floor_data.second;
	
	/*
	unsigned int floor_vao;
	glc(glGenVertexArrays(1, &floor_vao));
	glc(glBindVertexArray(floor_vao));

	unsigned int floor_vbo;
	glc(glGenBuffers(1, &floor_vbo));
	glc(glBindBuffer(GL_ARRAY_BUFFER, floor_vbo));
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floorVertices.size(), floorVertices.data(), GL_STATIC_DRAW));

	unsigned int floor_ebo;
	glc(glGenBuffers(1, &floor_ebo));
	glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo));
	glc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * floorIndices.size(), floorIndices.data(), GL_STATIC_DRAW));

	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));
	glc(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	glc(glEnableVertexAttribArray(1));
	glc(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	glc(glEnableVertexAttribArray(2));

	*/

	objId floorData = objectManager->registerObject();
	objectManager->mapVertexArray(floorData);
	objectManager->mapVertexBuffer(floorData, floorVertices.data(), floorVertices.size());
	objectManager->mapElementBuffer(floorData, floorIndices.data(), floorIndices.size());
	objectManager->expressVertexLayout(ObjectManager::VertexBufferLayout(std::vector<unsigned int>{3, 3, 2}));


	//Shader
	
	Shader cubeShader = Shader::parse("vertex", "fragment");
	cubeShader.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");

	Shader lightShader1 = Shader::parse("lightVertex", "lightFragment");
	lightShader1.set_mvpn("model_mat", "view_mat", "project_mat", "");

	Shader lightShader2 = Shader::parse("lightVertex", "lightFragment");
	lightShader2.set_mvpn("model_mat", "view_mat", "project_mat", "");

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

	cubeShader.attach();
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

	cubeShader.attach();
	Texture container = Texture::from_image("container2.png", cubeShader, "material.diffuse", 0);
	Texture container_specular = Texture::from_image("container2_specular.png", cubeShader, "material.specular", 1);

	floorShader.attach();
	Texture floor_diffuse = Texture::from_image("container2.png", floorShader, "material.diffuse", 0);
	Texture floor_specular = Texture::from_image("container2_specular.png", floorShader, "material.specular", 1);


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
		container.activate()->bind();
		container_specular.activate()->bind();
		cubeShader.view(view);
		cubeShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		for (size_t i = 0; i < n_Cubes; i++) {
			glm::mat4 mod = glm::mat4(1.0f);
			mod = glm::translate(mod, cubePositions[i]);
			mod = glm::rotate(mod, cubeRotations[i], cubeAxisOfRotations[i]);
			cubeShader.model(mod, true);
			glc(glBindVertexArray(objectManager->getVertexArray(cubeObjectData)));
			glc(glDrawArrays(GL_TRIANGLES, 0, 36));
			glc(glBindTexture(GL_TEXTURE_2D, 0));
		}
		
		
		floorShader.attach();
		floor_diffuse.activate()->bind();
		floor_specular.activate()->bind();
		floorShader.view(view);
		floorShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);

		
		glc(glBindVertexArray(objectManager->getVertexArray(floorData)));
		glc(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		glc(glBindTexture(GL_TEXTURE_2D, 0));

		//Draw lights
		for (const Light& light : lights) {
			light.shaderObj->attach();
			light.shaderObj->view(view);
			modelLight = glm::mat4(1.0f);
			modelLight = glm::translate(modelLight, light.position);
			modelLight = glm::scale(modelLight, glm::vec3(0.2f));
			light.shaderObj->model(modelLight, false);

			glc(glBindVertexArray(objectManager->getVertexArray(lightData)));
			glc(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
		
		
		//Unbind
		glc(glBindVertexArray(0));
		floorShader.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
