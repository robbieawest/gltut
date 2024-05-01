#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "helper.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "mouse.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


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


	float vertices[] = {
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
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	
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


	//Shader
	Shader shaderProgram = Shader::parse("vertex", "fragment");
	shaderProgram.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");

	Shader lightShader = Shader::parse("lightVertex", "lightFragment");
	lightShader.set_mvpn("model_mat", "view_mat", "project_mat", "");

	//MVP for object
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cam.pos, cam.pos + cam.dir, cam.up);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	shaderProgram.attach();
	shaderProgram.model(model, true);
	shaderProgram.view(view);
	shaderProgram.project(projection);
	shaderProgram.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
	shaderProgram.set_mat_uniforms("material.specular","material.shininess");
	shaderProgram.set_material(
		glm::vec3(1.0f, 0.5f, 0.31f),
		32.0f
	);
	shaderProgram.set_light_uniforms("light.ambient", "light.specular", "light.diffuse");
	shaderProgram.set_light(
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f)
	);

	glm::mat4 modelLight = glm::mat4(1.0f);

	glm::vec3 light_position(1.2f, 1.0f, 1.0f);
	shaderProgram.uniform3f("light.position", 1.2f, 1.0f, 1.0f);
	modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, light_position);
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));

	lightShader.attach();
	lightShader.model(modelLight, false);
	lightShader.view(view);
	lightShader.project(projection);

	shaderProgram.attach();
	Texture container = Texture::from_image("container2.png", shaderProgram, "material.diffuse", 0);


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glc(glEnable(GL_DEPTH_TEST));


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
		light_position.x = cos(glfwGetTime());
		light_position.y = cos(glfwGetTime() / 3.0f);
		light_position.z = sin(glfwGetTime());

		
		//Update and draw object
		container.activate()->bind();
		shaderProgram.attach();
		view = glm::lookAt(cam.pos, cam.pos + cam.dir, cam.up);
		shaderProgram.view(view);
		shaderProgram.uniform3f("light.position", light_position.x, light_position.y, light_position.z);
		shaderProgram.uniform3f("viewPos", cam.pos.x, cam.pos.y, cam.pos.z);
		
		glc(glBindVertexArray(VAO));
		glc(glDrawArrays(GL_TRIANGLES, 0, 36));
		glc(glBindTexture(GL_TEXTURE_2D, 0));

		//Draw light
		lightShader.attach();
		modelLight = glm::mat4(1.0f);
		modelLight = glm::translate(modelLight, light_position);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		lightShader.model(modelLight, false);
		lightShader.view(view);

		glc(glBindVertexArray(LVAO));
		glc(glDrawArrays(GL_TRIANGLES, 0, 36));
		
		//Unbind
		glc(glBindVertexArray(0));
		shaderProgram.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
