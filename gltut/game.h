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


	float cube_vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	
	//Create vertex array object to bind our VBO and vertex linking to
	unsigned int VAO;
	glc(glGenVertexArrays(1, &VAO));
	glc(glBindVertexArray(VAO));
	

	//Object
	unsigned int VBO;
	glc(glGenBuffers(1, &VBO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW));

	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));
	glc(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	glc(glEnableVertexAttribArray(1));
	glc(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))))
	glc(glEnableVertexAttribArray(2));
	

	//Light source
	unsigned int LVAO;
	glc(glGenVertexArrays(1, &LVAO));
	glc(glBindVertexArray(LVAO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));

	//Floor
	float floor_vertices[] = {
		-0.5f, 0.0f, 0.5f,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 100.0f,
		0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f, 100.0f, 100.0f,
		0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f, 100.0f, 0.0f,
	};

	unsigned int floor_indices[] = {
		0, 3, 2,
		2, 1, 0
	};

	unsigned int floor_vao;
	glc(glGenVertexArrays(1, &floor_vao));
	glc(glBindVertexArray(floor_vao));

	unsigned int floor_vbo;
	glc(glGenBuffers(1, &floor_vbo));
	glc(glBindBuffer(GL_ARRAY_BUFFER, floor_vbo));
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW));

	unsigned int floor_ebo;
	glc(glGenBuffers(1, &floor_ebo));
	glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo));
	glc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW));

	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));
	glc(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	glc(glEnableVertexAttribArray(1));
	glc(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	glc(glEnableVertexAttribArray(2));


	//Shader
	
	Shader cubeShader = Shader::parse("vertex", "fragment");
	cubeShader.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");

	Shader lightShader = Shader::parse("lightVertex", "lightFragment");
	lightShader.set_mvpn("model_mat", "view_mat", "project_mat", "");

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
			0.032f
		}
	};

	Light::updateShader(cubeShader, lights);
	Light::updateShader(floorShader, lights);

	glm::mat4 modelLight = glm::mat4(1.0f);

	cubeShader.attach();
	floorShader.attach();
	modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, lights[0].position);
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));

	lightShader.attach();
	lightShader.model(modelLight, false);
	lightShader.view(view);
	lightShader.project(projection);

	cubeShader.attach();
	Texture container = Texture::from_image("container2.png", cubeShader, "material.diffuse", 0);
	Texture container_specular = Texture::from_image("container2_specular.png", cubeShader, "material.specular", 1);

	floorShader.attach();
	Texture floor_diffuse = Texture::from_image("container2.png", floorShader, "material.diffuse", 0);
	Texture floor_specular = Texture::from_image("container2_specular.png", floorShader, "material.specular", 1);
	
	
	/*
	std::vector<float> sphereVertices = fibonacchi_sphere(150);
	unsigned int SVAO;
	glc(glGenVertexArrays(1, &SVAO));
	glc(glBindVertexArray(SVAO));

	//Object
	unsigned int SVBO;
	glc(glGenBuffers(1, &SVBO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, SVBO));
	glc(glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW));
	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));

	Shader sphereShader = Shader::parse("sphereVertex", "sphereFragment");
	sphereShader.set_mvpn("model_mat", "view_mat", "project_mat", "");
	sphereShader.attach();
	glm::mat4 modelSphere = glm::mat4(1.0f);
	modelSphere = glm::translate(modelSphere, glm::vec3(10.0f, 0.0f, 5.0f));
	modelSphere = glm::scale(modelSphere, glm::vec3(5.0f));
	sphereShader.model(modelSphere, false);
	sphereShader.view(view);
	sphereShader.project(projection);
	*/


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
	//game _loop_
	while (!glfwWindowShouldClose(window)) {
		
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		cam.handle_movement(deltaTime);

		//rendering
		glc(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Update light position
		lights[0].position = glm::vec3(
			3.0f * cos(glfwGetTime()),
			glm::abs(5.0f * cos(glfwGetTime() / 3.0f)),
			3.0f * sin(glfwGetTime())
		);
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
			glc(glBindVertexArray(VAO));
			glc(glDrawArrays(GL_TRIANGLES, 0, 36));
			glc(glBindTexture(GL_TEXTURE_2D, 0));
		}
		

		floorShader.attach();
		floor_diffuse.activate()->bind();
		floor_specular.activate()->bind();
		floorShader.view(view);
		floorShader.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);


		glc(glBindVertexArray(floor_vao));
		glc(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		glc(glBindTexture(GL_TEXTURE_2D, 0));

		//Draw light
		lightShader.attach();
		modelLight = glm::mat4(1.0f);
		modelLight = glm::translate(modelLight, lights[0].position);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		lightShader.model(modelLight, false);
		lightShader.view(view);

		glc(glBindVertexArray(LVAO));
		glc(glDrawArrays(GL_TRIANGLES, 0, 36));

		//Draw sphere
	//	sphereShader.attach();
	//	sphereShader.view(view);

	//	glc(glBindVertexArray(SVAO));
	//	glc(glDrawArrays(GL_POINTS, 0, sphereVertices.size()));
		
		//Unbind
		glc(glBindVertexArray(0));
		floorShader.deattach();
		//cubeShader.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
