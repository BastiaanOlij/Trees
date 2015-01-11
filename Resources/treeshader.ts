// Tessellation level is changed depending on our projected screen coordinates. 
// At some point we should add a check that if the tree is further away it would be good to skip tesselation alltogether.

#version 410 core

layout (vertices = 4) out;

const float precission = 25.0;
const float maxTessGenLevel = 16;
in VS_OUT {
	vec3 Vp;
	vec3 N;
	vec2 T;
} ts_in[];

out TS_OUT {
	vec3 N;
	vec2 T;
} ts_out[];

void main() {
	if (gl_InvocationID == 0) {
		// get our screen coords
		vec3 V0 = ts_in[0].Vp;
		vec3 V1 = ts_in[1].Vp;
		vec3 V2 = ts_in[2].Vp;
		vec3 V3 = ts_in[3].Vp;

		if ((V0.z <= 0.0) && (V1.z <= 0.0) && (V2.z <= 0.0) && (V3.z <= 0.0)) {
			// Behind the camera
			gl_TessLevelOuter[0] = 0;
			gl_TessLevelOuter[1] = 0;
			gl_TessLevelOuter[2] = 0;
			gl_TessLevelOuter[3] = 0;
			gl_TessLevelInner[0] = 0;
			gl_TessLevelInner[1] = 0;
		} else {
			float level0 = maxTessGenLevel;
			float level1 = maxTessGenLevel;
			float level2 = maxTessGenLevel;
			float level3 = maxTessGenLevel;

			if ((V0.z>0.0) && (V2.z>0.0)) {
				level0 = min(maxTessGenLevel, max(length(V0.xy - V2.xy) * precission, 1.0));
			};
			if ((V0.z>0.0) && (V1.z>0.0)) {
				level1 = min(maxTessGenLevel, max(length(V0.xy - V1.xy) * precission, 1.0));
			};
			if ((V1.z>0.0) && (V1.z>0.0)) {
				level2 = min(maxTessGenLevel, max(length(V1.xy - V3.xy) * precission, 1.0));
			};
			if ((V3.z>0.0) && (V2.z>0.0)) {
				level3 = min(maxTessGenLevel, max(length(V3.xy - V2.xy) * precission, 1.0));
			};

			gl_TessLevelOuter[0] = level0;
			gl_TessLevelOuter[1] = level1;
			gl_TessLevelOuter[2] = level2;
			gl_TessLevelOuter[3] = level3;
			gl_TessLevelInner[0] = min(level1, level3);
			gl_TessLevelInner[1] = min(level0, level2);
		};
	};

	// just copy our vertices as control points
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	ts_out[gl_InvocationID].N = ts_in[gl_InvocationID].N;
	ts_out[gl_InvocationID].T = ts_in[gl_InvocationID].T;
}