#version 410 core

layout(location = 0) in vec3 aPos;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 project_mat;

out vec3 FragPos;

void main() {
	gl_Position = project_mat * view_mat * model_mat * vec4(aPos, 1.0);
	FragPos = vec3(model_mat * vec4(aPos, 1.0));
}
