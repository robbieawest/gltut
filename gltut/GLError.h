#pragma once

#define STOP(x) if(!(x)) __debugbreak();

//Macro to wrap all gl calls
#define glc(x) GLClearError();\
	x;\
	STOP(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

void assert_cond(bool cond, std::string message);


//Miscellaneous
template<typename... Args>
std::string format(std::string s, Args... params);

template<typename ex, typename ...Args>
void throwError(std::string message, Args... params);
