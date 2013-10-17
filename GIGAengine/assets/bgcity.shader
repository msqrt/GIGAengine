
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 ex_uv;
smooth out float beat;

uniform vec2 screenSize;
uniform float t;


void main() {
	float theta = floor(t/2.0 + 0.5);

	mat2 yrot2d = mat2(
		cos(theta), sin(theta),
		-sin(theta), cos(theta)
	);

	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos*.5+vec2(.5);
	
	ex_uv.y *= -1;

	
	ex_uv = ex_uv*yrot2d;
	//ex_uv.y *= (870.0/515.0);
	ex_uv.y *= (515.0/870.0);
	ex_uv.y *= 1.0/ratio;
	ex_uv.y *= 2.0;
	ex_uv.x += t*0.1;
	ex_uv *= 0.6;
	
	vec2 position = ipos;
	
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));

	gl_Position = vec4(position,0.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 ex_uv;
smooth in float beat;
uniform sampler2D tex;
uniform float t;
uniform vec2 screenSize;

float gray(vec3 col) {
	 return dot(col, vec3(0.299, 0.587, 0.114));
}

void main() {
	vec3 luma = vec3(0.299, 0.587, 0.114);
	vec2 uv = ex_uv;

	// calculatd in VS
	//float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));

	vec4 col = texture2D(tex, uv );
	float jump = col.r - col.g;
	//float diff = sin((gl_FragCoord.x/screenSize.x)*4.0 + t);//(gl_FragCoord.x/screenSize.x); //;cos(beat*2.0*3.141 + 3.141592);
	//col = texture2D(tex, ex_uv + vec2(0.0, jump * 0.02 * diff) );
	
	col.rgb = col.rrr * vec3(0.95, 0.95, 1.0)*0.99;
	col.rgb = min(pow(col.rgb, vec3(18.0)), vec3(1.0));
	//

	//col.rgb = vec3(0.0, 1.0, 0.0);

	outcol = col;
}

#endif
