
#ifdef vertexcompile

layout (location=0) in vec3 ipos;
layout (location=1) in vec3 normal;
smooth out vec3 ex_Pos;
smooth out vec2 ex_uv;
smooth out vec3 ex_Normal;
smooth out float beat;

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 camera;


uniform float t;
uniform float speed;
uniform float spin;


void main() {
	float thet = t*0.05*speed;
	/*
	mat4 roty = mat4(
		cos(thet), 0.0, -sin(thet), 0.0,
		0.0, 1.0, 0.0, 0.0,
		sin(thet), 0.0, cos(thet), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	*/
	
	beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));
	

	mat3 roty = mat3(
		cos(thet), 0.0, -sin(thet),
		0.0, 1.0, 0.0,
		sin(thet), 0.0, cos(thet)
	);


	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos.xy*.5+vec2(.5);
	ex_uv.y *= -1;
	ex_uv.y *= (1024.0/919.0);
	ex_uv.y *= ratio;
	ex_uv *= 2.0;
	
	vec3 outpos = ipos;
	ex_Pos = outpos;
	outpos.y -=3.5;
	outpos *= 20.0;	// pallero isoks "skyboksiks"
	outpos.x += 1.0;
	
	outpos *= roty;
	outpos.x -= 4.0;
	
	float spin = distance(vec3(0.0), ipos) * pow(beat*2.0, 0.6)*0.05;
	
	mat3 roty2 = mat3(
		cos(spin), 0.0, -sin(spin),
		0.0, 1.0, 0.0,
		sin(spin), 0.0, cos(spin)
	);
	
	outpos *= roty2;
	outpos.y += 16.0;
	outpos.z += 32.0;
	
	ex_Normal = normal*roty;

	gl_Position =( projection*camera)*vec4(outpos, 1.0);
}

#endif

#ifdef fragmentcompile

#line 59
layout (location=0) out vec4 outcol;
smooth in vec3 ex_Pos;	// world space position
smooth in vec2 ex_uv;
smooth in vec3 ex_Normal;
smooth in float beat;
uniform sampler2D tex;
uniform float t;
uniform float lamp;

float gray(vec3 col) {
	 return dot(col, vec3(0.299, 0.587, 0.114));
}

vec3 hsv(float h, float s, float v)
{
	vec3 c = vec3(h,s,v);
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
	vec3 luma = vec3(0.299, 0.587, 0.114);
	vec3 light = normalize(vec3(0.1 + sin(t*0.08), 0.1, 0.8));
	float lamppu = lamp;
	
	float progress = mod(t, 1.0);
	// done in vertex shader
	//float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));
	
	vec2 plus = vec2(t*0.01 + beat * 0.03, beat*0.02);
	vec4 teks = texture2D(tex, ex_uv + plus );
	vec4 col = vec4(hsv(0.1 + ex_Pos.z*0.02 , 0.4, 1.1), 1.0);
	vec3 camvec = -normalize(ex_Pos);
	//col *= dot(light, reflect(normalize(ex_Normal.xyz + (teks.z-teks.x)*0.1)));
	vec3 norr = normalize(ex_Normal.xyz + (teks.z-teks.x)*0.1);
	float ambient = 0.1 + ex_Pos.z*0.9;
	col *= 1.0-(max(0.0, dot(light, reflect(camvec, norr))) + ambient + lamppu*dot(vec3(0.0, 1.0, 0.0), norr));
	col *= vec4(vec3(0.5), 1.0);

	//col = vec4(pow(col.rgb, vec3(1.5)), 1.0);
	
	outcol = clamp(vec4(pow(col.rgb, vec3(2.0)), 1.0),vec4(0.),vec4(10.0));
}

#endif
