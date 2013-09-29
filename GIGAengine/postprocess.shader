
#ifdef vertexcompile

layout(location=0) in vec2 pos;
smooth out vec2 uv;

void main() {
	uv = pos*.5+vec2(.5);
	gl_Position = vec4(pos.xy, .0, 1.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
layout(location=0) out vec4 outcol;

uniform sampler2D color, bloom, additional;

void main() {
	outcol = texture(color, uv);
}

#endif
