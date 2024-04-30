#version 410 core

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColour;

uniform vec3 objectColour;
uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

uniform Light light;


void main() {
	vec3 diffuse_map_vec = vec3(texture(material.diffuse, TexCoords));
	vec3 ambient = light.ambient * diffuse_map_vec;
	
	vec3 norm = normalize(normal);

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diffuse_map_vec * diff) * light.diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular * spec) * light.specular;

	FragColour = vec4(ambient + diffuse + specular, 1.0);
}