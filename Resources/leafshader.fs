#version 410 core

uniform sampler2D leafTexture;

in vec3 N;
in vec2 T;

out vec4 fragcolor;

void main() {
	vec4 color = texture(leafTexture, T);
	if (color.a < 0.8) {
		discard;
	} else if (color.r < 0.2 && color.g < 0.2 && color.b < 0.2) {
		discard;
	} else {
		// need to add a shadow check here and if we're in shadow, we apply our normal ambient factor
	
		vec3 L = normalize(vec3(1.0, 1.0, 1.0));
		float diffuse = dot(L, N);
		if (diffuse < 0.0) {
			// underside of our leaf? Leafs are translucent
			diffuse = -diffuse * 0.5;
		};
	  
	  	// apply our ambient lighting factor with a little more ambient
		float lightfactor = 0.6 + diffuse * 0.4;

		fragcolor = vec4(color.rgb * lightfactor, 1.0);
	};
}