
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

uniform sampler2D source;
uniform float dir;
uniform vec2 screen;

const float sigma = .5;

void main() {
	vec2 blurdir = vec2(dir, 1.0-dir)/screen;
	vec4 accum;
	float c;

	for(float i = .5; i<4.0; i++) {
		float a = exp(-i*i/(2*sigma*sigma));
		c += a;
		accum += texture(source, uv+i*blurdir)*a;
		accum += texture(source, uv-i*blurdir)*a;
	}

	outcol = accum/(sqrt(2.0*3.1415926535)*sigma)/c/2.0;
}

#endif
