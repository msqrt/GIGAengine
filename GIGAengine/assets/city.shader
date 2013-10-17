
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 ex_uv;
smooth out float beat;

uniform vec2 screenSize;
uniform float t;


void main() {
	float ratio = screenSize.y/screenSize.x;
	ex_uv = ipos*.5+vec2(.5);
	ex_uv.y *= -1;
	ex_uv.y *= (870.0/515.0);
	ex_uv.y *= ratio;
	ex_uv *= 1.0;
	ex_uv.x += t*0.01;	// scroll from left to right
	vec2 position = ipos;
	
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0));
	
	float action = mod(floor(t/2 + 0.5),3)*0.1;
	position *= 1.0 + sin(beat*1.0*3.141 + 3.141*0.5 - 0.5)*0.01;
	position.y *= 0.75;// + sin(ex_uv.x*40.0)*0.02;
	position.y -= 0.28 + pow(t*0.01, 2.0);
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
	
	col.rgb = pow(col.rgb, vec3(2.2));

	outcol = col;
}

#endif
