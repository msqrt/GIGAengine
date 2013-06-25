
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
flat out mat2 rot;
smooth out vec2 npos;
void main() {
	float s = 1/sqrt(2.0);
	rot = mat2(s, -s, s, s);
	npos = ipos*.5+vec2(.5);
	gl_Position = vec4(ipos,.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outpos;
layout (location=1) out vec4 outcol;
flat in mat2 rot;
smooth in vec2 npos;
uniform sampler2D col, pos;

uniform float t;

void main() {
	vec4 scol = texture2D(col, npos), spos = texture2D(pos, npos);
	
	vec3 tpos = spos.xyz;
	tpos.xz *= rot;
	vec3 f = vec3(cos(tpos.xyz*10.0+.5*t));
	tpos.yz *= rot;
	vec3 g = vec3(cos(tpos.xyz*50.0+1.0*t));
	spos.xyz += pow(7.0, -scol.w)*scol.w*8.0*spos.w*mix(.03*vec3(f.z-f.y, f.x-f.z,-.01-abs(f.y-f.x)),.01*vec3(g.z-g.y, g.x-g.z,-abs(g.y-g.x)),.7-scol.w*spos.w);

	outcol = vec4(scol.xyz,scol.w-.03);
	outpos = spos;
}

#endif
