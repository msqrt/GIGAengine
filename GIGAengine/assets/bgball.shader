
#ifdef vertexcompile

layout (location=0) in vec3 ipos;
layout (location=1) in vec3 normal;
//smooth out vec4 ex_Pos;
smooth out vec2 ex_uv;
smooth out vec3 ex_Normal;

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 camera;

uniform float t;


void main() {
	float thet = t*0.05;
	/*
	mat4 roty = mat4(
		cos(thet), 0.0, -sin(thet), 0.0,
		0.0, 1.0, 0.0, 0.0,
		sin(thet), 0.0, cos(thet), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	*/
	

	mat3 roty = mat3(
		cos(thet), 0.0, -sin(thet),
		0.0, 1.0, 0.0,
		sin(thet), 0.0, cos(thet)
	);


	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos.xy*.5+vec2(.5);
	ex_uv.y *= -1;
	ex_uv.y *= (1024.0/1120.0);
	ex_uv.y *= ratio;
	
	ex_Normal = normal;
	
	vec3 outpos = ipos;
	outpos.y -=3.5;
	outpos *= 20.0;	// pallero isoks "skyboksiks"
	outpos.x += 1.0;
	
	outpos *= roty;
	outpos.x -= 4.0;

	gl_Position =( projection*camera)*vec4(outpos, 1.0);
}

#endif

#ifdef fragmentcompile

#line 59
layout (location=0) out vec4 outcol;
smooth in vec2 ex_uv;
smooth in vec3 ex_Normal;
uniform sampler2D tex;
uniform float t;

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
	vec3 light = normalize(vec3(0.1 + sin(t*0.08), 0.3, 0.4));
	
	vec2 plus = vec2(t*0.03, 0.0);
	vec4 teks = texture2D(tex, ex_uv + plus );
	vec4 col = vec4(hsv(0.6, 0.3, 0.8), 1.0);
	col *= dot(light, ex_Normal.xyz + teks.xyz*0.1);

	outcol = vec4(pow(col.rgb, vec3(2.0)), 1.0);
}

#endif
