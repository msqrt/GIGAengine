
#ifdef vertexcompile

layout (location=0) in vec2 pos;

smooth out vec2 uv;

void main() {
	uv = vec2(.5)+.5*pos;
	uv.y *= -1;
	gl_Position = vec4(pos, 0.0, 1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 uv;

uniform sampler2D tex;

void main() {
	vec4 col =texture(tex, uv);
	outcol = vec4(pow(col.rgb, vec3(2.2)), col.a);
}

#endif
