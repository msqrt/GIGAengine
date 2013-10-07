
uniform vec2 screen;

#ifdef vertexcompile

layout(location=0) in vec2 pos;
smooth out vec2 uv;

void main() {
	uv = pos*.5+vec2(.5)-vec2(.5)/screen.xy;
	gl_Position = vec4(pos.xy, .0, 1.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
layout(location=0) out vec4 outcol;

uniform sampler2D source;
uniform float i;

void main() {
	vec2 b = vec2(.5, -.5);
	vec4 c = textureLod(source, uv+b.yy/screen.xy, i)+
			 textureLod(source, uv+b.xy/screen.xy, i)+
			 textureLod(source, uv+b.yx/screen.xy, i)+
			 textureLod(source, uv+b.xx/screen.xy, i);
	outcol = c*.25;
}

#endif
