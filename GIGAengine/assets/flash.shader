
#ifdef vertexcompile

layout (location=0) in vec2 pos;


void main() {

	gl_Position = vec4(pos, .0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
uniform float bright;


void main() {

	outcol = vec4(1.0,1.0,1.0,bright);
}

#endif
