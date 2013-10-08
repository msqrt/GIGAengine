
#ifdef vertexcompile

uniform sampler2D pos, col;
uniform float t, aspect;
layout (location=0) in vec2 coord;
smooth out vec4 pcol;

void main() {
	vec4 psample = texture2D(pos, coord), csample = texture2D(col, coord);
	gl_PointSize = 4.0*(pow(2.0, csample.w)-1.1);
	float ang = .1*sin(t*.5);
	vec3 ppos = psample.xyz;
	ppos.xz *= mat2(cos(ang),sin(ang),-sin(ang),cos(ang));
	ang = .1*sin(t*.5+1.5);
	ppos.yz *= mat2(cos(ang),sin(ang),-sin(ang),cos(ang));
	ppos = vec3(4.0*ppos.xy,ppos.z);
	pcol = vec4(csample.xyz*pow(2.71828,-ppos.z*5.0),1.0);
	gl_Position = vec4(ppos.x/(2.5+ppos.z)/16.0*9.0*aspect,.1+ppos.y/(2.5+ppos.z),ppos.z,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec4 pcol;

void main() {
	if(length(gl_PointCoord-vec2(.5))>.5)
		discard;
	outcol = pcol;
}

#endif
