
uniform vec2 screen;

#ifdef vertexcompile

layout(location=0) in vec2 pos;
smooth out vec2 uv;

void main() {
	uv = pos*.5+vec2(.5)+vec2(.25)/screen.xy;
	gl_Position = vec4(pos.xy, .0, 1.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
layout(location=0) out vec4 outcol;

uniform sampler2D source;
uniform float dir;
uniform float i;

const float sigma = .47;

void main() {
	vec2 blurdir = vec2(2.0)*vec2(dir, 1.0-dir)/screen.xy;
	vec4 accum = vec4(.0);
	float c = 1.0;
	accum += textureLod(source, uv, i);
	for(float j = .5; j<5.5; ++j) {
		float a = exp(-j*j/(2*sigma*sigma));
		c += 2.0*a;
		accum += textureLod(source, uv+j*blurdir, i)*a;
		accum += textureLod(source, uv-j*blurdir, i)*a;
	}
	outcol = accum/(sqrt(2.0*3.1415926535)*sigma)/c;
}

#endif
