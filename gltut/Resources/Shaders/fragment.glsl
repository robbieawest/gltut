#version 410 core

in vec3 normal;
in vec3 FragPos;
out vec4 FragColour;

uniform vec3 objectColour;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColour;

	FragColour = vec4((ambient + diffuse + specular) * objectColour, 1.0);
}