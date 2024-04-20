#pragma once
#include "helper.h"
#include <fstream>
#include <iostream>
#include <unordered_map>


class Shader {

private:

	unsigned int id = -1;
	std::unordered_map<std::string, int> uniform_cache;

	static std::string get_source(std::string filename) {
		std::string path = format_g("./Resources/Shaders/{}.glsl", filename);

		std::ifstream parser(path);
		if (!parser.is_open()) 
			spdlog::error("shader does not exist at path: {}", path);
		
		std::string ret = "";
		std::string line;
		while (std::getline(parser, line))
			ret += line + '\n';
		
		parser.close();
		return ret;
	}

public:

	Shader() {}

	static Shader parse(std::string vertex_path, std::string fragment_path) {

		//Parse paths and get sources
		std::string vertex_source = get_source(vertex_path);
		const char* vertex_input = vertex_source.c_str();
		std::string fragment_source = get_source(fragment_path);
		const char* fragment_input = fragment_source.c_str();

		//Create vertex shader
		glc(unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER));
		glc(glShaderSource(vertex_shader, 1, &vertex_input, nullptr));
		glc(glCompileShader(vertex_shader));

		//Check vertex shader compilation
		int success;
		char infolog[512];
		glc(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
		if (!success) {
			glc(glGetShaderInfoLog(vertex_shader, 512, nullptr, infolog));
			spdlog::error("|VERTEX SHADER COMPILATION ERROR|\n{}", infolog);
		}


		//Create fragment shader
		glc(unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
		glc(glShaderSource(fragment_shader, 1, &fragment_input, nullptr));
		glc(glCompileShader(fragment_shader));

		//Check fragment shader compilation
		glc(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
		if (!success) {
			glc(glGetShaderInfoLog(fragment_shader, 512, nullptr, infolog));
			spdlog::error("|FRAGMENT SHADER COMPILATION ERROR|\n{}", infolog);
		}

		Shader ret;

		//Create shader program
		glc(ret.id = glCreateProgram());
		glc(glAttachShader(ret.id, vertex_shader));
		glc(glAttachShader(ret.id, fragment_shader));
		glc(glLinkProgram(ret.id));

		//Check shader linking
		glc(glGetProgramiv(ret.id, GL_LINK_STATUS, &success));
		if (!success) {
			glc(glGetProgramInfoLog(ret.id, 512, nullptr, infolog));
			spdlog::error("|SHADER PROGRAM LINKING ERROR|\n{}", infolog);
		}

		ret.attach();
		glc(glDeleteShader(fragment_shader));
		glc(glDeleteShader(vertex_shader));

		spdlog::info("shader parsed without error from {}.glsl and {}.glsl", vertex_path, fragment_path);

		return ret;
	}

	void attach() {
		glc(glUseProgram(id));
	}

	void deattach() {
		glc(glUseProgram(0));
	}

	void uniform4f(std::string uniform_id, float f1, float f2, float f3, float f4) {
		
		if (uniform_cache.find(uniform_id) == uniform_cache.end()) {
			//Not cached
			const char* uniform_id_cstr = uniform_id.c_str();
			glc(int uniform_location = glGetUniformLocation(id, uniform_id_cstr));
			if (uniform_location == -1) {
				spdlog::warn("uniform {} is not associated with shader", uniform_id);
				return;
			}

			uniform_cache.insert({ uniform_id, uniform_location });
		}

		glc(glUniform4f(uniform_cache[uniform_id], f1, f2, f3, f4));
	}
};
