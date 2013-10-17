
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
	float thet = t*0.5;
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
	
	vec3 outpos = ipos*roty;
	gl_Position =( projection*camera)*vec4(outpos, 1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 ex_uv;
smooth in vec3 ex_Normal;
uniform sampler2D tex;
uniform float t;

float gray(vec3 col) {
	 return dot(col, vec3(0.299, 0.587, 0.114));
}

void main() {
	vec3 luma = vec3(0.299, 0.587, 0.114);
	
	vec4 col = texture2D(tex, ex_uv );
	col = vec4(ex_Normal.xyz, 1.0);

	outcol = vec4(pow(col.rgb, vec3(2.0)), 1.0);
}

#endif
