
#ifdef vertexcompile

layout (location=0) in vec2 ivalue;
uniform float row, rowh;
smooth out vec2 npos;

void main() {
	vec2 ipos = vec2(ivalue.x, (ivalue.y+1.0)*rowh+row);
	npos = ipos*.5+vec2(.5);
	gl_Position = vec4(ipos,.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outpos;
layout (location=1) out vec4 outcol;
smooth in vec2 npos;
uniform sampler2D tex1, tex2;

uniform float t;

void main() {
	vec3 rpos = fract(999.9*cos(99.9*vec3(dot(vec3(npos,t),vec3(11.1,13.7,19.9)),dot(vec3(npos,t),vec3(12.5,13.1,11.9)),dot(vec3(npos,t),vec3(14.1,15.2,17.7)))));
	vec4 smp1 = texture2D(tex1, rpos.xy),
		 smp2 = texture2D(tex2, rpos.xy);
	float d = length(smp1.xyz-smp2.xyz)/length(vec3(1.0));
	outpos = vec4((rpos.xy-vec2(.5)),(rpos.z-.5)*.05,d);
	outcol = vec4(smp1.xyz,.9+.2*rpos.z);
}

#endif
