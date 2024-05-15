#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <unordered_map>

#include "spdlog/spdlog.h"
#include "shader.h"

typedef unsigned int objId;

class ObjectManager {
private:
	objId nextObjectId = 0;
	std::unordered_map<objId, GLuint> vertexArrayMappings;
	std::unordered_map<objId, GLuint> vertexBufferMappings;
	std::unordered_map<objId, GLuint> elementBufferMappings;
	std::unordered_map<objId, Shader> shaderProgramMappings;

	ObjectManager() {}
	~ObjectManager() {
		delete instance;
	}

	static ObjectManager* instance;

	bool validateId(objId _id) {
		if (_id > nextObjectId) {
			spdlog::error("_id {} not registered with ObjectMapper.", _id);
			return false;
		}

		return true;
	}

public:
	static ObjectManager* getInstance() {
		if (instance == nullptr) {
			instance = new ObjectManager();
		}
		return instance;
	}

	unsigned int registerObject() {
		return nextObjectId++;
	}

	unsigned int getVertexArray(objId _id) {
		if (!validateId(_id)) return -1;

		auto vArrayMap = vertexArrayMappings.find(_id);
		if (vArrayMap == vertexArrayMappings.end()) {
			spdlog::error("getVertexArray: _id {} contains no vertex array mappings.", _id);
			return -1;
		}

		return vArrayMap->second;
	}

	unsigned int getVertexBuffer(objId _id) {
		if (!validateId(_id)) return -1;

		auto vBufMap = vertexBufferMappings.find(_id);
		if (vBufMap == vertexBufferMappings.end()) {
			spdlog::error("getVertexBuffer: _id {} contains no vertex buffer mappings.", _id);
			return -1;
		}

		return vBufMap->second;
	}

	unsigned int getElemenetBuffer(objId _id) {
		if (!validateId(_id)) return -1;

		auto vElementMap = elementBufferMappings.find(_id);
		if (vElementMap == elementBufferMappings.end()) {
			spdlog::error("getElementBuffer: _id {} contains no element buffer mappings.", _id);
			return -1;
		}

		return vElementMap->second;
	}

	Shader* getShaderProgram(objId _id) {
		if (!validateId(_id)) return nullptr;

		auto vShaderMap = shaderProgramMappings.find(_id);
		if (vShaderMap == shaderProgramMappings.end()) {
			spdlog::error("getShaderProgram: _id {} contains no shader program mappings.", _id);
			return nullptr;
		}

		return &vShaderMap->second;
	}
};
