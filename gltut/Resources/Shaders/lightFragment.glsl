#version 410 core

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

in vec3 normal;
in vec3 FragPos;
out vec4 FragColour;

void main() {
	FragColour = vec4(light.ambient + light.diffuse + light.specular, 1.0);
}
