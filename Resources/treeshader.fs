#version 410 core

uniform mat3 normalMat;
uniform sampler2D treeTexture;

in TE_OUT {
	vec3 tangent;
	vec3 bitangent;
	vec2 T;
} fs_in;

out vec4 fragcolor;

void main() {
	vec4 color = texture(treeTexture, fs_in.T);

	vec3 L = normalize(vec3(1.0, 1.0, 1.0));
	vec3 N = normalMat * cross(-normalize(fs_in.tangent), normalize(fs_in.bitangent));
	float diffuse = max(0.0, dot(L, N));
  
	// apply our ambient lighting factor
	float lightfactor = 0.4 + diffuse * 0.6;

	fragcolor = vec4(color.rgb * lightfactor, 1.0);
}