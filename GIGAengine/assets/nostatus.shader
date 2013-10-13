
uniform float t;

#ifdef vertexcompile

layout(location=0) in vec2 pos;
uniform vec2 screenSize;
uniform int index;
smooth out vec2 uv;
smooth out vec4 exColor;

void main() {
	uv = pos*.5+vec2(.5);
	exColor = vec4(0.5+sin(index)*0.5, 0.5-cos(index*0.5)*0.5, float(index%3)/8.0, 1.0);
	gl_Position = vec4(pos.xy * vec2(screenSize.y/screenSize.x, 1.0), .0, 1.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
smooth in vec4 exColor;
layout(location=0) out vec4 outcol;

void main() {
	vec3 c = vec3(.0);
	
	c = vec3(.1,.5,1.0)*.125*(2.0+sin(uv.x+t*2.1)+cos(uv.y+t*1.9));

	outcol = exColor;
}

#endif
