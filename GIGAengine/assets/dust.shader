
vec3 hsltorgb(vec3 hsl) {
	float c = (1.0-abs(2.0*hsl.z-1.0))*hsl.y;
	hsl.x /= 60.0;
	float x = c*(1-abs(mod(hsl.x,2.0)-1.0));
	vec3 ret = vec3(x*float((hsl.x<2.0 && hsl.x>=1.0) || (hsl.x>=4.0 && hsl.x<5.0))+c*float(hsl.x<1.0 || hsl.x>=5.0),
				x*float(hsl.x<1.0 || (hsl.x>=3.0 && hsl.x<4.0))+c*float(hsl.x>=1.0 && hsl.x<3.0),
				x*float(hsl.x>=5.0 || (hsl.x>=2.0 && hsl.x<3.0))+c*float(hsl.x>=3.0 && hsl.x<5.0))+vec3(hsl.z-.5*hsl.y);
	return ret;
}

mat4 getyrot(float alpha) {
	return mat4(
		cos(alpha), 0.0, sin(alpha), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(alpha), 0.0, cos(alpha), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

uniform float t;
#ifdef vertexcompile

layout(location=0) in vec4 apos;
layout(location=1) in vec3 rot1;
layout(location=2) in vec3 rot2;

smooth out vec3 nor, dir;
smooth out vec4 pos;
smooth out float l;
flat out vec3 col;

uniform mat4 projection, camera;

void main() {
	gl_PointSize = min(1.0,(t*.1+10.0*apos.w-10.0))*8.0*rot2.z*rot2.z;
	l = float(gl_PointSize<.0);
	float x = mod(apos.x+t*.05*(.5+.5*rot2.x),1.0);
	float r = min(1.0,t*.001);
	col = vec3(.1);
	if(rot2.z>.9)
		col = hsltorgb(vec3(apos.y*20.0, .1, .5));
	if(apos.w>.9)
		col = hsltorgb(vec3(140+apos.y*20.0, .1, .5));
	mat4 cam = camera;
	
	//cam = transpose(cam);
	
	float alpha = t*0.11;
	float t2 = t/2.0;
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0)) ;
	float beat2 = floor(t2) + smoothstep(0.0, 1.0, mod(t2, 1.0)) ;
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	
	mat4 rotationy4 = getyrot(t*0.1 + pos.y*0.02);

	vec3 ppos = vec3(vec3(vec3(x,apos.z*2.0+sin(x*10.0+rot2.y)*6.0+sin(x*50.0+rot2.y*2.0+t)*3.0,sin(x*5.0+rot2.z+t)*5.0)-vec3(.5))*vec3(-2.0,r*.1,r*.1)).yxz*10.0;
	ppos.y += 0.0 - beat2*0.4;
	ppos.x += -4.0 + 4.0 * (mod(floor(t/8), 3 + mod(floor(t/9), 2)));
	ppos *= 2.0;
	
	ppos = (rotationy4 * vec4(ppos, 1.0)).xyz;
	gl_Position = (projection*cam)*vec4(ppos, 1.0);
	//gl_Position = projection*camera*vec4(ppos,1.0);
}

#endif

#ifdef fragmentcompile

layout(location=0) out vec4 outcol;
layout(location=1) out vec4 outadd;

smooth in vec3 nor, dir;
smooth in vec4 pos;
smooth in float l;
flat in vec3 col;

uniform vec3 primaryDirection, primaryColour, secondaryDirection, secondaryColour;

void main() {
	if(length(gl_PointCoord-vec2(.5))>.5 || l > .0)
		discard;
	outcol = vec4(col,1.0);
	outadd = vec4(.0);
}

#endif
