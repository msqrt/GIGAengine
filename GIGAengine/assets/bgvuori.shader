
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 ex_uv;

uniform vec2 screenSize;
uniform float t;


void main() {
	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos*.5+vec2(.5);
	ex_uv.y *= -1;
	ex_uv.y *= (1024.0/1120.0);
	ex_uv.y *= ratio;
	ex_uv *= 1.0;
	ex_uv.y -= t*0.01;
	
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
	vec2 plus = vec2(0.0, sin(ex_uv.x)*sin(ex_uv.y*4.0 + t*0.5)*0.01);
	vec4 col = texture2D(tex, ex_uv + plus);
	vec4 col2 = texture2D(tex, ex_uv + plus + vec2(0.0, pow(gray(col.rgb), 4.0 ) * 0.05) * jump);
	col = (col + col2) / 2.0;
	vec3 sky = vec3(0.1, sin(ex_uv.y*2.0 + t*0.1)*0.2, 0.7);

	
	
	outcol = vec4(
		mix(pow(col.rgb, vec3(2.5)), sky, 
			 clamp((1.0-dot(col.rgb*2.0, luma*4.0)), 0.0, 1.0)	// blend the background only to black parts
		),
	1.0);
}

#endif
