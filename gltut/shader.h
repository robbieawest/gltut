#pragma once
#include "helper.h"
#include <fstream>
#include <iostream>
#include <unordered_map>


class Shader {

private:

	unsigned int id = -1;
	std::string m = "", v = "", p = "", n = "";
	std::string specular, shininess;
	std::string lambient, lspecular, ldiffuse;
	bool mat_uni_def = false;
	bool light_uni_def = false;
	bool mvpn_def = false;
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

	void cache_uniform(std::string uniform_id) {
		if (uniform_cache.find(uniform_id) == uniform_cache.end()) {
			//Not cached
			const char* uniform_id_cstr = uniform_id.c_str();
			glc(int uniform_location = glGetUniformLocation(id, uniform_id_cstr));
			if (uniform_location == -1) {
				spdlog::warn("uniform '{}' is not associated with shader", uniform_id);
				return;
			}

			uniform_cache.insert({ uniform_id, uniform_location });
		}
	}


	void uniform4f(std::string uniform_id, float f1, float f2, float f3, float f4) {
		cache_uniform(uniform_id);

		glc(glUniform4f(uniform_cache[uniform_id], f1, f2, f3, f4));
	}

	void uniform3f(std::string uniform_id, float f1, float f2, float f3) {
		cache_uniform(uniform_id);

		glc(glUniform3f(uniform_cache[uniform_id], f1, f2, f3));
	}

	void uniform1i(std::string uniform_id, int i) {
		cache_uniform(uniform_id);

		glc(glUniform1i(uniform_cache[uniform_id], i));
	}

	void uniform1f(std::string uniform_id, float i) {
		cache_uniform(uniform_id);

		glc(glUniform1f(uniform_cache[uniform_id], i));
	}

	void uniformMatrix4f(std::string uniform_id, glm::mat4 mat) {
		cache_uniform(uniform_id);

		glc(glUniformMatrix4fv(uniform_cache[uniform_id], 1, false, glm::value_ptr(mat)));
	}

	void uniformMatrix3f(std::string uniform_id, glm::mat3 mat) {
		cache_uniform(uniform_id);

		glc(glUniformMatrix3fv(uniform_cache[uniform_id], 1, false, glm::value_ptr(mat)));
	}

	void model(glm::mat4 model_mat, bool normal_mat) {
		if (!mvpn_def) {
			spdlog::warn("mvpn not set for model");
			return;
		}
		uniformMatrix4f(m, model_mat);
		if (normal_mat) {
			if (n == "")
				spdlog::warn("normal matrix attempted to be changed when n is not set");
			else
				uniformMatrix3f(n, glm::mat3(glm::transpose(glm::inverse(model_mat))));
		}
	}

	void view(glm::mat4 view_mat) {
		if (!mvpn_def) {
			spdlog::warn("mvpn not set for model");
			return;
		}
		uniformMatrix4f(v, view_mat);
	}

	void project(glm::mat4 project_mat) {
		if (!mvpn_def) {
			spdlog::warn("mvpn not set for model");
			return;
		}
		uniformMatrix4f(p, project_mat);
	}
	
	void set_mvpn(std::string m, std::string v, std::string p, std::string n) {
		mvpn_def = true;
		cache_uniform(m);
		cache_uniform(v);
		cache_uniform(p);
		this->m = m;
		this->v = v;
		this->p = p;

		if (n == "")return;
		cache_uniform(n);
		this->n = n;
	}

	void set_mat_uniforms(std::string spec, std::string shi) {
		mat_uni_def = true;
		cache_uniform(spec);
		cache_uniform(shi);

		specular = spec;
		shininess = shi;
	}

	void set_material(glm::vec3 spec, float shi) {
		if (!mat_uni_def) {
			spdlog::warn("material uniforms have not been defined.");
			return;
		}

		uniform3f(specular, spec.x, spec.y, spec.z);
		uniform1f(shininess, shi);
	}

	void set_light_uniforms(std::string amb, std::string spec, std::string diff) {
		light_uni_def = true;
		cache_uniform(amb);
		cache_uniform(spec);
		cache_uniform(diff);

		lambient = amb;
		lspecular = spec;
		ldiffuse = diff;
	}

	void set_light(glm::vec3 amb, glm::vec3 spec, glm::vec3 diff) {
		if (!light_uni_def) {
			spdlog::warn("light uniforms have not been defined.");
			return;
		}

		uniform3f(lambient, amb.x, amb.y, amb.z);
		uniform3f(lspecular, spec.x, spec.y, spec.z);
		uniform3f(ldiffuse, diff.x, diff.y, diff.z);
	}
	
};
