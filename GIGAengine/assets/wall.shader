
vec3 hsltorgb(vec3 hsl) {
	float c = (1.0-abs(2.0*hsl.z-1.0))*hsl.y;
	hsl.x /= 60.0;
	float x = c*(1-abs(mod(hsl.x,2.0)-1.0));
	vec3 ret = vec3(x*float((hsl.x<2.0 && hsl.x>=1.0) || (hsl.x>=4.0 && hsl.x<5.0))+c*float(hsl.x<1.0 || hsl.x>=5.0),
				x*float(hsl.x<1.0 || (hsl.x>=3.0 && hsl.x<4.0))+c*float(hsl.x>=1.0 && hsl.x<3.0),
				x*float(hsl.x>=5.0 || (hsl.x>=2.0 && hsl.x<3.0))+c*float(hsl.x>=3.0 && hsl.x<5.0))+vec3(hsl.z-.5*hsl.y);
	return ret;
}

#ifdef vertexcompile

layout(location=0) in vec3 apos;
layout(location=1) in vec3 altan;
layout(location=2) in vec3 artan;

smooth out vec3 nor, dir;
smooth out vec4 pos;

uniform mat4 projection, camera;
uniform float t;

void main() {
	vec3 loc = apos*10000.0+sin(vec3(t)+apos*1000.0)*5.0;
	vec3 loc2 = altan*10000.0+sin(vec3(t)+altan*1000.0)*5.0;
	vec3 loc3 = artan*10000.0+sin(vec3(t)+artan*1000.0)*5.0;
	pos = camera*vec4(loc,1.0);
	nor = vec4(camera*vec4(-normalize(cross(loc2-loc,loc3-loc)),.0)).xyz;
	dir = reflect(pos.xyz,nor);
	gl_Position = projection*pos;
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;
layout(location=1) out vec4 outadd;

smooth in vec3 nor, dir;
smooth in vec4 pos;

uniform vec3 primaryDirection, primaryColour, secondaryDirection, secondaryColour;

void main() {
	float fres = clamp(.0+.2*pow(1.0-dot(normalize(pos.xyz),-normalize(nor)),15.0),.0,1.0);
	outcol = mix(vec4(.01), vec4((dot(normalize(dir),normalize(primaryDirection))*.5+.5)*hsltorgb(primaryColour) + (dot(normalize(dir),normalize(secondaryDirection))*.5+.5)*hsltorgb(secondaryColour), 1.0), fres);
	outadd = vec4(.0);
}

#endif
