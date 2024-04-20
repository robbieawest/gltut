#version 410 core

out vec4 outcol;
in vec3 col;

void main() {
	outcol = vec4(col, 1.0);
}