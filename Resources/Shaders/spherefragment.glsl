#version 410 core

in vec3 normal;
in vec3 FragPos;
out vec4 FragColour;

void main() {
	FragColour = vec4(1.0);
	//FragColour = vec4(dot(normal, vec3(1.0)));
}
