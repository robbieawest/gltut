#include <GL/glew.h>
#include <iostream>
#include <format>
#include <string>
#include "spdlog/spdlog.h"


//Miscellaneous
template<typename... Args>
std::string format(std::string s, Args... params) {
	return std::vformat(s, std::make_format_args(params...));
}

template<typename ex, typename ...Args>
void throwError(std::string message, Args... params) {
	std::string out = format(message, params...);
	spdlog::error(out);
	throw ex(out);
}

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		throwError<std::runtime_error>("Line {} {}\n| OpenGL ERROR | : [{}] {}", line, file, error, function);
	}
	return true;
}

void assert_cond(bool cond, std::string message) {
	if (!cond) 
		throwError<std::runtime_error>("Assertion error |{}| -> cond was false", message);
	
}
