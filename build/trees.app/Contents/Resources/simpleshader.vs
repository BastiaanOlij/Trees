#version 330

uniform mat4 mvp;
layout (location=0) in vec3	vertices;

void main() {
	vec4 V = vec4(vertices, 1.0);
	gl_Position = mvp * V;
}
