#pragma once
#include "helper.h"
#include "shader.h"

void mainLoop() {
	assert_cond(window, "window setup check");

	float vertices[] = {
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5
	};

	//Create vertex array object to bind our VBO and vertex linking to
	unsigned int VAO;
	glc(glGenVertexArrays(1, &VAO));
	glc(glBindVertexArray(VAO));

	//Create vertex buffer for triangle
	unsigned int VBO;
	glc(glGenBuffers(1, &VBO));
	glc(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	glc(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	unsigned int EBO;
	glc(glGenBuffers(1, &EBO));
	glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	glc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	
	//Link vertex attributes
	glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	glc(glEnableVertexAttribArray(0));
	glc(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	glc(glEnableVertexAttribArray(1));

	glc(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	Shader shaderProgram = Shader::parse("vertex", "fragment");


	//game _loop_
	while (!glfwWindowShouldClose(window)) {

		//inputs
		processInputs();

		//rendering
		glc(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		glc(glClear(GL_COLOR_BUFFER_BIT));


		shaderProgram.attach();

		//Draw trianges
		glc(glBindBuffer(GL_ARRAY_BUFFER, VAO));
		glc(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

		//Unbind
		glc(glBindBuffer(GL_ARRAY_BUFFER, 0));
		shaderProgram.deattach();

		//Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
