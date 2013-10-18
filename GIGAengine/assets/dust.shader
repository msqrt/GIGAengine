
vec3 hsltorgb(vec3 hsl) {
	float c = (1.0-abs(2.0*hsl.z-1.0))*hsl.y;
	hsl.x /= 60.0;
	float x = c*(1-abs(mod(hsl.x,2.0)-1.0));
	vec3 ret = vec3(x*float((hsl.x<2.0 && hsl.x>=1.0) || (hsl.x>=4.0 && hsl.x<5.0))+c*float(hsl.x<1.0 || hsl.x>=5.0),
				x*float(hsl.x<1.0 || (hsl.x>=3.0 && hsl.x<4.0))+c*float(hsl.x>=1.0 && hsl.x<3.0),
				x*float(hsl.x>=5.0 || (hsl.x>=2.0 && hsl.x<3.0))+c*float(hsl.x>=3.0 && hsl.x<5.0))+vec3(hsl.z-.5*hsl.y);
	return ret;
}

uniform float t;
#ifdef vertexcompile

layout(location=0) in vec4 apos;
layout(location=1) in vec3 rot1;
layout(location=2) in vec3 rot2;

smooth out vec3 nor, dir;
smooth out vec4 pos;
smooth out float l;

uniform mat4 projection, camera;

void main() {
	gl_PointSize = min(1.0,(t*.1+10.0*apos.w-10.0))*5.0;
	l = float(gl_PointSize<.0);
	float x = mod(apos.x+t*.05*(.5+.5*rot2.x),1.0);
	float r = min(1.0,t*.001);
	gl_Position = projection*vec4(vec3(vec3(x,sin(x*10.0+rot2.y)*10.0+sin(x*50.0+rot2.y)*5.0,sin(x*5.0+rot2.z)*5.0)-vec3(.5))*vec3(2.0,r*.1,r*.1)-vec3(.0,.0,.1+r*.9),1.0);
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;
layout(location=1) out vec4 outadd;

smooth in vec3 nor, dir;
smooth in vec4 pos;
smooth in float l;

uniform vec3 primaryDirection, primaryColour, secondaryDirection, secondaryColour;

void main() {
	if(length(gl_PointCoord-vec2(.5))>.5 || l > .0)
		discard;
	outcol = vec4(.2);
	outadd = vec4(.0);
}

#endif
