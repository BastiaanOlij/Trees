#version 410 core

layout (location=0) in vec3	vertices;
layout (location=1) in vec3	normals;
layout (location=2) in vec2	texcoords;

uniform mat4 mvp;

out VS_OUT {
	vec3 Vp;
	vec3 N;
	vec2 T;
} vs_out;

void main() {
	vec4 V = vec4(vertices, 1.0);
	gl_Position = V;
	V = mvp * V;
	vs_out.Vp = V.xyz / V.w;
	vs_out.N = normals;
	vs_out.T = texcoords;
}