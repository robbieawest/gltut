#version 410 core

in vec3 normal;
in vec3 fragPos;
out vec4 FragColour;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPos;


void main() {
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColour;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColour;

	FragColour = vec4((ambient + diffuse) * objectColour, 1.0);
}