
#ifdef vertexcompile

layout(location=0) in vec2 pos;

void main() {
	gl_Position = vec4(pos.xy, .0, 1.0);
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;

void main() {
	outcol = vec4(1.0, .0, 1.0, 1.0);
}

#endif
