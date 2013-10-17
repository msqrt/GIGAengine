
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 ex_uv;

uniform vec2 screenSize;
uniform float t;


void main() {
	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos*.5+vec2(.5);
	
	float theta = 0.10;
	
	mat2 yrot2d = mat2(
		cos(theta), sin(theta),
		-sin(theta), cos(theta)
	);

	ex_uv.y *= -1;
	ex_uv.y *= (1024.0/1120.0);
	ex_uv.y *= ratio;
	
	ex_uv = ex_uv * yrot2d;
	
	ex_uv *=1.5;
	ex_uv.y += 0.42 - t*0.0020;
	ex_uv.x += -0.1;

	gl_Position = vec4(ipos,.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 ex_uv;
uniform sampler2D tex;
uniform float t;

float gray(vec3 col) {
	 return dot(col, vec3(0.299, 0.587, 0.114));
}

void main() {
	vec3 luma = vec3(0.299, 0.587, 0.114);
	
	// wobble
	float jump = 0.5*sin(t*1.0 + ex_uv.x*2.0)*0.5;
	vec2 plus = vec2(0.0, 0.0);
	vec4 col = texture2D(tex, ex_uv + plus);
	//vec4 col2 = texture2D(tex, ex_uv + plus + vec2(0.0, pow(gray(col.rgb), 4.0 ) * 0.05) * jump);
	//col = (col + col2) / 2.0;
	//vec3 sky = vec3(0.1, sin(ex_uv.y*2.0 + t*0.1)*0.2, 0.7);

	outcol = vec4(
		pow(col.rgb * vec3(0.3, 0.3, 0.3), vec3(2.0)),
		0.8*clamp((dot(col.rgb*2.0, luma*4.0)), 0.0, 1.0)
		);
}

#endif
