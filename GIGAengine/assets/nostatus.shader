
uniform float t;

#ifdef vertexcompile

layout(location=0) in vec3 pos;

layout(location=1) in vec3 normal;

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 camera;

uniform int index;
smooth out vec2 uv;
smooth out vec4 exColor;
smooth out vec3 exNormal;

mat4 getyrot(float alpha) {
	return mat4(
		cos(alpha), 0.0, sin(alpha), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(alpha), 0.0, cos(alpha), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

void main() {
	mat4 cam = camera;
	
	float thet = t*0.5;
	mat2 rotation = mat2(
	cos(thet), -sin(thet), 
	sin(thet), cos(thet)
	);
	
	float alpha = t*0.2;
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	
	mat4 rotationy4 = getyrot(floor(t/4));
	cam = cam * rotationy4;

	
	vec2 screenratio = vec2(screenSize.y/screenSize.x, 1.0);
	uv = pos.xy*.5+vec2(.5);
	 uv *= 0.5;
	
	/*
	uv *= screenratio;
	uv *= vec2(1.2, 1.0);
	
	uv += vec2(0.7, 0.28);
	*/
	//uv += vec2(t*(0.1 + 0.1*sin(float(index))), 0.0);
	
	vec2 outpos = (rotation*(pos.xy))*screenratio;
	//uv += vec2(sin(outpos.x*2.0), sin(outpos.y*3.0)) * 0.5;
	//uv += vec2(distance(vec2(0.0), outpos.xy));
	

	vec3 ppos = pos;
	float glitchtime = t/2.0 + 0.5 ;
	ppos.x = ppos.x + ppos.y*sin(floor(glitchtime) * 2.0);

	float c = max(0.0, 1.0-mod(glitchtime, 1.0)*8.0)*0.2;	// flashy
	exColor = vec4(vec3(c), 1.0);
	gl_Position = (projection*cam)*vec4(ppos, 1.0);
	
	outpos = gl_Position.xy;
	ppos *= 4.0;
	
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
smooth in vec4 exColor;
smooth in vec3 exNormal;
layout(location=0) out vec4 outcol;
uniform sampler2D tex;

void main() {
	vec3 c = texture(tex, uv).rgb;
		vec3 light = normalize(vec3(1.0, 0.5, 0.2));
	//c *= dot(light, exNormal);
	
	//c = vec3(.1,.5,1.0)*.125*(2.0+sin(uv.x+t*2.1)+cos(uv.y+t*1.9));

	
	outcol = vec4(pow(c, vec3(2.2)), 1.0) + exColor;
	outcol.b = gl_FragCoord.z;
}

#endif
