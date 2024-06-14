#pragma once
#include "ResourceManager.h"
#include "shader.h"
#include "texture.h"
#include "helper.h"
#include "spdlog/spdlog.h"

class RenderInstance {
private:
	bool init = false, ebuf = false;
	objId resourceId;
	unsigned int numElements = -1;
	Shader shader;
	std::vector<Texture> textures = std::vector<Texture>();
	Resource::ResourceManager* resourceManagerInstance;

public:
	RenderInstance(Shader shader) : shader(shader){
		resourceManagerInstance = Resource::ResourceManager::getInstance();
		resourceId = resourceManagerInstance->registerObject();
	}

	RenderInstance(std::string vertexPath, std::string fragmentPath){
		shader = Shader::parse(vertexPath, fragmentPath);
		shader.set_mvpn("model_mat", "view_mat", "project_mat", "normal_mat");
		resourceManagerInstance = Resource::ResourceManager::getInstance();
		resourceId = resourceManagerInstance->registerObject();
	}

	void addTexture(Texture texture) {
		textures.push_back(texture);
	}

	void loadTexture(std::string path, std::string uniform) {
		Texture newTexture = Texture::from_image(path, shader, uniform);
		textures.push_back(newTexture);
	}

	void readData(std::string modelPath, Resource::VertexBufferLayout layout) {
		vertex_data vertexData = load_vertex_data(modelPath); //The error handling here sucks ballsacks
		std::vector<float> vbufData = vertexData.first;
		std::vector<unsigned int> ebufData = vertexData.second;

		if (vbufData.empty()) {
			spdlog::warn("RenderInstance::readData: model path does not contain any vertex data - ignored");
			return;
		}

		resourceManagerInstance->mapVertexArray(resourceId);
		resourceManagerInstance->mapVertexBuffer(resourceId, vbufData.data(), vbufData.size());
		if (ebufData.size() != 0) {
			resourceManagerInstance->mapElementBuffer(resourceId, ebufData.data(), ebufData.size());
			ebuf = true;
		}
		resourceManagerInstance->expressVertexLayout(layout);
		
		if (ebuf) {
			numElements = ebufData.size();
		}
		else {
			numElements = vbufData.size() / (layout.getStride() / sizeof(float));
		}
		init = true;
	}

	Shader& getShader() {
		return shader;
	}

	void render() {
		if (!init) {
			spdlog::warn("Render Instance not initialized on render() call, ignoring.");
			return;
		}

		shader.attach();
		for (Texture& texture : textures) {
			texture.activate()->bind();
		}

		glc(glBindVertexArray(resourceManagerInstance->getVertexArray(resourceId)));

		if (ebuf) {
			glc(glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0));
		}
		else {
			glc(glDrawArrays(GL_TRIANGLES, 0, numElements));
		}
		
	}

	//TODO Add functionality for texture wrapping settings

};
