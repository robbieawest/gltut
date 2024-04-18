#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "helper.h"
#include "GLError.h"
#include "game.h"


int main() {

	init(); //Setup
	mainLoop();
	return exit();
}