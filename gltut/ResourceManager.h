#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <unordered_map>

#include "spdlog/spdlog.h"
#include "shader.h"
#include "texture.h"
#include "GLError.h"

typedef unsigned int objId;

namespace Resource {
	
	class VertexBufferLayout {
	private:
		std::vector<unsigned int> properties;
		unsigned int stride = -1;

		unsigned int calculateStride() {
			unsigned int ret = 0;
			for (int i : properties) ret += i;
			return ret * sizeof(float);
		}

		VertexBufferLayout() {}

	public:
		VertexBufferLayout(std::vector<unsigned int> p) : properties(p) {
			stride = 0;
			for (int i : properties) stride += i;
			stride *= sizeof(float);
		}

		const unsigned int getStride() const {
			return stride;
		}

		std::vector<unsigned int>& getProperties() {
			return properties;
		}

		static VertexBufferLayout tightlyPacked(unsigned int propertySize) {
			VertexBufferLayout ret;
			ret.stride = 0;
			ret.properties.push_back(propertySize);
			return ret;
		}
	};

	class ResourceManager {
	private:
		const unsigned int MAX_TEXTURES = 32;
		objId nextObjectId = 0;
		//Maybe combine these all into one map
		std::unordered_map<objId, GLuint> vertexArrayMappings;
		std::unordered_map<objId, GLuint> vertexBufferMappings;
		std::unordered_map<objId, GLuint> elementBufferMappings;
		
		ResourceManager() {
		}

		~ResourceManager() {
			delete instance;
		}

		static ResourceManager* instance;

		bool validateId(objId _id) const {
			if (_id > nextObjectId) {
				spdlog::error("ObjectManager: _id {} not registered with ObjectMapper.", _id);
				return false;
			}

			return true;
		}

	public:
		static ResourceManager* getInstance() {
			if (instance == nullptr) {
				instance = new ResourceManager();
			}
			return instance;
		}

		objId registerObject() {
			return nextObjectId++;
		}

		void mapVertexArray(objId _id) {
			validateId(_id);

			auto vBuf = vertexBufferMappings.find(_id);
			auto eBuf = elementBufferMappings.find(_id);

			GLuint vao;
			glc(glGenVertexArrays(1, &vao));
			glc(glBindVertexArray(vao));

			if (vBuf != vertexBufferMappings.end()) {
				glc(glBindBuffer(GL_ARRAY_BUFFER, vBuf->second));
			}

			if (eBuf != elementBufferMappings.end()) {
				glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuf->second));
			}

			vertexArrayMappings.insert(std::make_pair(_id, vao));
		}

		void mapSharedVertexArray(objId _id, objId shareFrom) {
			validateId(_id);
			validateId(shareFrom);

			auto vArr = vertexArrayMappings.find(shareFrom);
			if (vArr == vertexArrayMappings.end()) {
				spdlog::error("mapSharedVertexArray: No vertex array mapping exists of object id: {}.", shareFrom);
				return;
			}

			vertexBufferMappings.insert(std::make_pair(_id, vArr->second));
		}

		void mapVertexArrayFromSharedComponents(objId _id, objId shareFrom) {
			validateId(_id);
			validateId(shareFrom);

			auto vBuf = vertexBufferMappings.find(shareFrom);
			auto eBuf = elementBufferMappings.find(shareFrom);

			GLuint vao;
			glc(glGenVertexArrays(1, &vao));
			glc(glBindVertexArray(vao));

			bool usedSharedComponent = false;
			if (vBuf != vertexBufferMappings.end()) {
				glc(glBindBuffer(GL_ARRAY_BUFFER, vBuf->second));
				usedSharedComponent = true;
			}

			if (eBuf != elementBufferMappings.end()) {
				glc(glBindBuffer(GL_ARRAY_BUFFER, eBuf->second));
				usedSharedComponent = true;
			}
			else if (!usedSharedComponent) spdlog::warn("mapVertexArrayFromSharedComponents: No shared components found from {}, vertex array still created.", shareFrom);

			vertexArrayMappings.insert(std::make_pair(_id, vao));
		}

		GLuint getVertexArray(objId _id) {
			if (!validateId(_id)) return -1;

			auto vArrayMap = vertexArrayMappings.find(_id);
			if (vArrayMap == vertexArrayMappings.end()) {
				spdlog::error("getVertexArray: _id {} contains no vertex array mappings.", _id);
				return -1;
			}

			return vArrayMap->second;
		}

		void mapVertexBuffer(objId _id, void* bufferData, size_t bufferSize) {
			validateId(_id);

			GLuint vbo;
			glc(glGenBuffers(1, &vbo));
			glc(glBindBuffer(GL_ARRAY_BUFFER, vbo));
			glc(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bufferSize, bufferData, GL_STATIC_DRAW));
			

			vertexBufferMappings.insert(std::make_pair(_id, vbo));
		}

		void expressVertexLayout(VertexBufferLayout layout) {
			std::vector<unsigned int> vertexProperties = layout.getProperties();
			unsigned int offset = 0;
			for (size_t i = 0; i < vertexProperties.size(); ++i) {
				unsigned int property = vertexProperties[i];
				glc(glVertexAttribPointer(i, property, GL_FLOAT, GL_FALSE, layout.getStride(), (void*)(offset * sizeof(float))));
				glc(glEnableVertexAttribArray(i));

				offset += property;
			}
		}

		void mapSharedVertexBuffer(objId _id, objId shareFrom) {
			validateId(_id);
			validateId(shareFrom);

			auto vBuf = vertexBufferMappings.find(shareFrom);
			if (vBuf == vertexBufferMappings.end()) {
				spdlog::error("mapSharedVertexBuffer: No vertex buffer mapping exists of object id: {}.", shareFrom);
				return;
			}

			vertexBufferMappings.insert(std::make_pair(_id, vBuf->second));
		}

		GLuint getVertexBuffer(objId _id) {
			if (!validateId(_id)) return -1;

			auto vBufMap = vertexBufferMappings.find(_id);
			if (vBufMap == vertexBufferMappings.end()) {
				spdlog::error("getVertexBuffer: _id {} contains no vertex buffer mappings.", _id);
				return -1;
			}

			return vBufMap->second;
		}

		void mapElementBuffer(objId _id, void* bufferData, size_t bufferSize) {
			validateId(_id);

			unsigned int ebo;
			glc(glGenBuffers(1, &ebo));
			glc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
			glc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * bufferSize, bufferData, GL_STATIC_DRAW));

			elementBufferMappings.insert(std::make_pair(_id, ebo));
		}

		void mapSharedElementBuffer(objId _id, objId shareFrom) {
			validateId(_id);
			validateId(shareFrom);

			auto eBuf = elementBufferMappings.find(shareFrom);
			if (eBuf == elementBufferMappings.end()) {
				spdlog::error("mapSharedElementBuffer: No element buffer mapping exists of object id: {}.", shareFrom);
				return;
			}

			vertexBufferMappings.insert(std::make_pair(_id, eBuf->second));
		}

		GLuint getElementBuffer(objId _id) {
			if (!validateId(_id)) return -1;

			auto vElementMap = elementBufferMappings.find(_id);
			if (vElementMap == elementBufferMappings.end()) {
				spdlog::error("getElementBuffer: _id {} contains no element buffer mappings.", _id);
				return -1;
			}

			return vElementMap->second;
		}

	};
}
