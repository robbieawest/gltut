#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 project_mat;
uniform mat3 normal_mat;

out vec3 normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
	gl_Position = project_mat * view_mat * model_mat * vec4(aPos, 1.0);
	normal = normal_mat * aNormal;
	FragPos = vec3(model_mat * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
}