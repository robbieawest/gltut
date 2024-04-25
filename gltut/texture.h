#pragma once
#include <string>
#include "stb/stb_image.h"//uhh dont think about this 
#include "spdlog/spdlog.h"
#include "GLError.h"
#include "shader.h"

class Texture {
private:
	bool defined = false;
	unsigned int id;
	unsigned int type = 0;
	int width, height, nrChannels;
public:

	static void flip() {
		stbi_set_flip_vertically_on_load(true);
	}

	static Texture from_image(std::string filename, Shader& shaderProgram, std::string uniform_id, unsigned int glid) {
		std::vector<std::string> spl_res = split(filename, '.');
		if (spl_res.size() != 2)spdlog::error("Invalid filename format: {}", filename);
		std::string extension = spl_res[1];
		if (extension != "png" && extension != "jpg")spdlog::error("Invalid file extension: {}", extension);

		std::string path = format_g("./Resources/Images/{}", filename);
		const char* path_cstr = path.c_str();

		Texture ret;
		unsigned char* im_data = stbi_load(path_cstr, &ret.width, &ret.height, &ret.nrChannels, 0);
		
		//Create texture
		glc(glGenTextures(1, &ret.id));

		if (glid > 31)spdlog::error("Texture glid cannot be greater than 31");
		ret.type = GL_TEXTURE0 + glid;

		glc(glActiveTexture(ret.type));
		glc(glBindTexture(GL_TEXTURE_2D, ret.id));
		shaderProgram.uniform1i(uniform_id, glid);

		glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		glc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		
		//Generate texture
		if (im_data) {
			unsigned int type = extension == "png" ? GL_RGBA : GL_RGB;
			glc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ret.width, ret.height, 0, type, GL_UNSIGNED_BYTE, im_data));
			glc(glGenerateMipmap(GL_TEXTURE_2D));
		}
		else {
			spdlog::error("Texture not loaded. File: {}", filename);
		}

		stbi_image_free(im_data);
		
		spdlog::info("Texture successfully loaded from file {}", path);
		ret.defined = true;
		return ret;
	}


	Texture* activate() {
		if (type == 0 || !defined)spdlog::error("Texture activated before definition or type set");
		glc(glActiveTexture(type));
		return this;
	}

	Texture* bind() {
		if (!defined)spdlog::error("Texture bound before definition");
		glc(glBindTexture(GL_TEXTURE_2D, id));
		return this;
	}

};
