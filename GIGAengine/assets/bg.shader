
#ifdef vertexcompile

layout(location=0) in vec3 pos;
layout(location=1) in vec3 nor;
layout(location=2) in vec3 rnd;

uniform mat4 projection, camera;

void main() {
	gl_Position = projection*camera*vec4(pos.xyz, 1.0);
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;
layout(location=1) out vec4 outadd;

void main() {
	outcol = vec4(1.0);
	outadd = vec4(.0);
}

#endif
