#version 410 core

layout (location=0) in vec3	vertices;
layout (location=1) in vec3	normals;
layout (location=2) in vec2	texcoords;

uniform mat4 mvp;
uniform mat3 normalMat;

out vec3 N;
out vec2 T;

void main() {
	gl_Position = mvp * vec4(vertices, 1.0);
	N = normalize(normalMat * normals);
	T = texcoords;
}