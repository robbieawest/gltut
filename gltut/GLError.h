#pragma once
#include "spdlog/spdlog.h"
#include "GL/glew.h"

//Miscellaneous

template<typename... Args>
std::string format_g(std::string s, Args... params) {
	return std::vformat(s, std::make_format_args(params...));
}
/*
template<typename ex, typename ...Args>
void throwError(std::string message, Args... params) {
	std::string out = format_g(message, params...);
	spdlog::error(out);
	throw ex(out);
}
*/

//Debugging
#define STOP(x) if(!(x)) __debugbreak();

//Macro to wrap all gl calls
#define glc(x) GLClearError();\
	x;\
	STOP(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		spdlog::error("Line {} {}\n| OpenGL ERROR | : [{}] {}", line, file, error, function);
	}
	return true;
}

void assert_cond(bool cond, std::string message) {
	if (!cond) 
		spdlog::error("Assertion error |{}| -> cond was false", message);
	
}
