
#ifdef vertexcompile

layout(location=0) in vec2 pos;
smooth out vec2 uv;

void main() {
	uv = pos*.5+vec2(.5);
	gl_Position = vec4(pos.xy, .0, 1.0);
}

#endif

#ifdef fragmentcompile

uniform sampler2D color, bloom, additional;
uniform float t;
smooth in vec2 uv;
layout(location=0) out vec4 outcol;

void main() {
	outcol = texture(color, uv)+
			 textureLod(bloom,uv,.0)*9.0+
			 textureLod(bloom,uv,2.0)*4.0+
			 textureLod(bloom,uv,3.0)*2.0+
			 textureLod(bloom,uv,4.0)*1.0+
			 textureLod(bloom,uv,5.0)*.5;
	outcol *= 1.0-.05*fract(9992.41*cos(10.9*(uv.x*19.9+uv.y*15.1+t*1.97)));
}

#endif