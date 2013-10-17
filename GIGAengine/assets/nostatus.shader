
uniform float t;

#ifdef vertexcompile

layout(location=0) in vec3 pos;

layout(location=1) in vec3 normal;

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 camera;
uniform float extrarotation;	// used with multiple drawcalls

uniform int index;
smooth out vec2 uv;
smooth out vec4 exColor;
smooth out vec4 exNormal;
smooth out float wobble;

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

	float alpha = t*0.2;
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0)) ;
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	
	mat4 rotationy4 = getyrot(floor(t/4) + t*0.05 + extrarotation * 1.0);
	cam = cam * rotationy4;

	
	vec2 screenratio = vec2(screenSize.y/screenSize.x, 1.0);
	uv = pos.xy*.5+vec2(.5);
	uv *= 0.25;


	vec3 ppos = pos;
	float glitchtime = t/2.0 + 0.5 ;
	ppos.x = ppos.x + ppos.y*sin(floor(glitchtime) * 2.0);
	ppos.x += mod(t, 4.0);

	float c = max(0.0, 1.0-mod(glitchtime, 1.0)*8.0)*0.1;	// flashy
	exColor = vec4(vec3(c), 1.0);
	
	gl_Position = (projection*cam)*vec4(ppos, 1.0);
	exNormal = normalize((projection*cam)*vec4(normal, 1.0)* vec4(1.0, 1.0, 0.00, 1.0));
	wobble = sin(length(gl_Position));
	
	ppos *= 4.0;
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
smooth in vec4 exColor;
smooth in vec4 exNormal;
smooth in float wobble;
layout(location=0) out vec4 outcol;
uniform sampler2D tex;

void main() {
	vec2 plus = vec2(sin(0.008*t), 0.0);
	vec3 c = texture(tex, uv + plus).rgb;
	vec3 light = normalize(vec3(0.5 + sin(t*0.9)*0.5, 0.5, 0.2));

	
	c = c * vec3(0.9, 0.7, 0.5);
	outcol = vec4(pow(c, vec3(2.2)), 1.0) + exColor;
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	diffuse += max(0.0, dot(light, exNormal.xyz * 4.0)) * 4.0;
	diffuse += 2.0 * vec3(0.1, 0.0, 0.9) * max(0.0, dot(-light, exNormal.xyz * 4.0));
	outcol.rgb = pow(c, vec3(2.2)) * diffuse;


}

#endif
