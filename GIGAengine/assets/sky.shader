
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

layout(location=0) in vec3 pos;

smooth out vec3 nor;

uniform mat4 projection, camera;

void main() {
	nor = -pos;
	gl_Position = projection*camera*vec4(pos*-10.0, 1.0);
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;
layout(location=1) out vec4 outadd;

smooth in vec3 nor;

uniform vec3 primaryDirection, primaryColour, secondaryDirection, secondaryColour;

void main() {
	outcol = vec4((dot(normalize(nor),normalize(primaryDirection))*.5+.5)*hsltorgb(primaryColour) + (dot(normalize(nor),normalize(secondaryDirection))*.5+.5)*hsltorgb(secondaryColour), 1.0);
	outadd = vec4(.0);
}

#endif
