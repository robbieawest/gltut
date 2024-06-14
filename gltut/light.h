#pragma once
#include "glm/glm.hpp"
#include "shader.h"
#include "GLError.h"
#include "RenderInstance.h"
#include "ResourceManager.h"

class Light {
private:
	glm::vec3 position, ambient, specular, diffuse;
	float constant, linear, quadratic;
	glm::vec3(*updateFunction) (float);
	Shader* shaderObj;
	RenderInstance renderInstance;
public:

	Light(glm::vec3 p, glm::vec3 a, glm::vec3 s, glm::vec3 d, float c, float l, float q, Shader* sh, glm::vec3(*f)(float), std::string data_path, Resource::VertexBufferLayout layout)
		: position(p)
		, ambient(a)
		, specular(s)
		, diffuse(s)
		, constant(c)
		, linear(l)
		, quadratic(q)
		, shaderObj(sh)
		, updateFunction(f) {
		renderInstance.readData(data_path, layout);
	}

	Shader* getShader() {
		return shaderObj;
	}

	void updatePosition(float time) {
		position = updateFunction(time);
	}

	void updateFragmentUniforms() {
		shaderObj->attach();
		shaderObj->uniform3f("light.ambient", ambient.x, ambient.y, ambient.z);
		shaderObj->uniform3f("light.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shaderObj->uniform3f("light.specular", specular.x, specular.y, specular.z);
	}

	static void updateShader(Shader& shaderProgram, std::vector<Light> lights) {
		shaderProgram.attach();
		shaderProgram.uniform1i("nPointLights", lights.size());
		for (size_t i = 0; i < lights.size(); i++) {
			std::string lightId = format_g("lights[{}].", i);
			shaderProgram.uniform3f(lightId + "position", lights[i].position.x, lights[i].position.y, lights[i].position.z);
			shaderProgram.uniform3f(lightId + "diffuse", lights[i].diffuse.x, lights[i].diffuse.y, lights[i].diffuse.z);
			shaderProgram.uniform3f(lightId + "ambient", lights[i].ambient.x, lights[i].ambient.y, lights[i].ambient.z);
			shaderProgram.uniform3f(lightId + "specular", lights[i].specular.x, lights[i].specular.y, lights[i].specular.z);
			shaderProgram.uniform1f(lightId + "constant", lights[i].constant);
			shaderProgram.uniform1f(lightId + "constant", lights[i].constant);
			shaderProgram.uniform1f(lightId + "linear", lights[i].linear);
			shaderProgram.uniform1f(lightId + "quadratic", lights[i].quadratic);
		}
	}

	static void updateShaderOnlyPositions(Shader& shaderProgram, std::vector<Light>& lights) {
		shaderProgram.attach();
		for (size_t i = 0; i < lights.size(); i++) {
			shaderProgram.uniform3f(format_g("lights[{}].position", i), lights[i].position.x, lights[i].position.y, lights[i].position.z);
		}
	}

	void render(glm::mat4 view) {
		shaderObj->attach();
		shaderObj->view(view);

		glm::mat4 model(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(0.2f));
		shaderObj->model(model, false);
		renderInstance.render();
	}
};
