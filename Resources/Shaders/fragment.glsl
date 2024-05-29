#version 410 core

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColour;

uniform vec3 objectColour;
uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	float constant;
	float linear;
	float quadratic;
};

#define MAX_POINT_LIGHTS 32;
uniform int nPointLights;
uniform Light lights[32];

vec3 processPointLight(Light light, vec3 norm, vec3 viewDir) {

	vec3 diffuse_map_vec = vec3(texture(material.diffuse, TexCoords));
	vec3 ambient = light.ambient * diffuse_map_vec;
	
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diffuse_map_vec * diff) * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular_map_vec = vec3(texture(material.specular, TexCoords));
	vec3 specular = specular_map_vec * spec * light.specular;

	float distance = length(FragPos - light.position);
	float attenuation = 1.0f / (
		light.constant + light.linear * distance + light.quadratic * distance * distance
	);
	
	return attenuation * (diffuse + ambient + specular);
}


void main() {
	
	vec3 resultantLight = vec3(0.0);
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	for(int i = 0; i < nPointLights; i++) {
		resultantLight += processPointLight(lights[i], norm, viewDir);
	}

	FragColour = vec4(resultantLight, 1.0);
}