
uniform float t;
uniform vec3 col;

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

void main() {
	vec3 c = vec3(.0);
	
	c = col*.25*(2.0+sin(uv.x+t*2.1)+cos(uv.y+t*1.9));

	outcol = vec4(c, 1.0);
}

#endif
