
uniform float t;

#ifdef vertexcompile

layout(location=0) in vec3 pos;

uniform vec2 screenSize;
uniform mat4 camera;
uniform mat4 projection;

uniform int index;
smooth out vec2 uv;
smooth out vec4 exColor;

void main() {
	float thet = t*0.5;
	mat2 rotation = mat2(
	cos(thet), -sin(thet), 
	sin(thet), cos(thet)
	);
	
	float alpha = t*0.2;
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	
	vec2 screenratio = vec2(screenSize.y/screenSize.x, 1.0);
	vec2 outpos = pos.xy*screenratio;
	uv = pos.xy*.5+vec2(.5);
	/*
	uv = pos*.5+vec2(.5);
	uv *= screenratio;
	uv *= vec2(1.2, 1.0);
	uv *= 2.5;
	uv += vec2(0.7, 0.28);
	//uv += vec2(t*(0.1 + 0.1*sin(float(index))), 0.0);
	
	vec2 outpos = (rotation*(pos.xy))*screenratio;
	uv += vec2(sin(outpos.x*2.0), sin(outpos.y*3.0)) * 0.5;
	//uv += vec2(distance(vec2(0.0), outpos.xy));
	*/
	
	outpos*=1.0;
	exColor = vec4(0.5+sin(index)*0.5, 0.5-cos(index*0.5)*0.5, float(index%3)/8.0, 1.0);
	gl_Position =   projection*camera*vec4(pos, 1.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
smooth in vec4 exColor;
layout(location=0) out vec4 outcol;
uniform sampler2D tex;

void main() {
	vec3 c = vec3(.0);
	
	c = vec3(.1,.5,1.0)*.125*(2.0+sin(uv.x+t*2.1)+cos(uv.y+t*1.9));
	

	outcol = vec4(pow(texture(tex, uv).rgb, vec3(2.2)), 1.0);
}

#endif
