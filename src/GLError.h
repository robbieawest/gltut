#pragma once
#include "GL/glew.h"

#include <sstream>
#include <vector>
#include <iterator>
#include <string>

#include "spdlog/spdlog.h"

//Miscellaneous

template<typename... Args>
std::string format_g(std::string s, Args... params) {
	return std::vformat(s, std::make_format_args(params...));
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
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
