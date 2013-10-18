
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
	
	

	float alpha = t*0.11;
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0)) ;
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	vec3 norm = normal;
	
	mat4 rotationy4 = getyrot(t*0.02 + extrarotation * 1.0);
	cam = cam * rotationy4;
	norm = (vec4(norm, 1.0) * rotationy4).xyz;

	vec2 screenratio = vec2(screenSize.y/screenSize.x, 1.0);
	uv = pos.xy*.5+vec2(.5);
	uv *= 0.25;

	vec3 ppos = pos;
	ppos.y += extrarotation * 16.0;
	float glitchtime = t/2.0 + 0.5 ;
	//ppos.x = ppos.x + ppos.y*sin(floor(glitchtime) * 2.0);
	//ppos.x += mod(t, 4.0);

	float c = max(0.0, 1.0-mod(glitchtime, 1.0)*8.0)*0.1;	// flashy
	exColor = vec4(vec3(c), 1.0);
	
	//cam[3][1] += -t*1;	// y translation
	ppos.y += -t*1.2;
	ppos.x += t*0.3;
	gl_Position = (projection*cam)*vec4(ppos, 1.0);
	exNormal = normalize((projection*cam)*vec4(norm, 1.0)* vec4(1.0, 1.0, 0.00, 1.0));
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
	c.gb += vec2(0.1, 0.2);
	c.rgb *= 0.5;
	vec3 light = normalize(vec3(0.2 , 0.7, 0.0));

	
	c = c * vec3(0.9, 0.7, 0.5);
	c = c * vec3(2.5);
	outcol = vec4(pow(c, vec3(2.2)), 1.0) + exColor;
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 joku_normal = normalize(vec3(1.0, 1.0, 0.1) * exNormal.xyz * 4.0);
	diffuse += max(-1.0, dot(light, joku_normal.xyz * 8.0)) * 1.0;
	//diffuse += 2.0 * vec3(0.1, 0.0, 0.9) * max(0.0, dot(-light, exNormal.xyz * 4.0));
	outcol.rgb = pow(c, vec3(2.2)) * diffuse;
	//outcol.rgb = joku_normal.rgb;
	
	outcol.rgb *= pow(clamp(dot(light, joku_normal)*0.1, 0.0, 1.0), 2.0) * 256.0;

}

#endif
