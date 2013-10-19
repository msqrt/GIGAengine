
uniform float t;

#ifdef vertexcompile

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 camera;
uniform float extrarotation;	// used with multiple drawcalls

uniform float index;
smooth out vec2 uv;
smooth out vec4 exColor;
smooth out vec4 exNormal;
smooth out float wobble;
smooth out float dist;

mat4 getyrot(float alpha) {
	return mat4(
		cos(alpha), 0.0, sin(alpha), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(alpha), 0.0, cos(alpha), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 getTranslation(vec3 ofs)
{
	mat4 t;
	t[0][3] = ofs.x;
	t[1][3] = ofs.y;
	t[2][3] = ofs.z;
	
	return t;
}

mat4 createLookatMatrix(vec3 eye, vec3 target, vec3 up_vec)
{
    vec3 forward, side, up;
	mat4 m;

	forward = target - eye;
	up = up_vec;

	forward = normalize(forward);

    /* Side = forward x up */
    side = normalize(cross(forward, up));

    /* Recompute up as: up = side x forward */
    up = cross(side, forward);

    m = mat4(1.0); // identity

    m[0][0] = side.x;
    m[0][1] = side.y;
    m[0][2] = side.z;

    m[1][0] = up.x;
    m[1][1] = up.y;
    m[1][2] = up.z;

    m[2][0] = -forward.x;
    m[2][1] = -forward.y;
    m[2][2] = -forward.z;

	m = getTranslation(vec3(-eye.x, -eye.y, -eye.z)) * m;
    //m.translate(-eye.x, -eye.y, -eye.z);

    return m;
}

void main() {
	//createLookatMatrix(vec3 eye, vec3 target, vec3 up_vec)
	/*
	mat4 cam = createLookatMatrix(
		vec3(0.0, 0.0, 0.0), 
		vec3(0.0, 0.0, -100.0),
		vec3(0.0, 1.0, 0.0)
	);
	*/
	mat4 cam = camera;
	
	//cam = transpose(cam);
	
	float alpha = t*0.11;
	float t2 = t/2.0;
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0)) ;
	float beat2 = floor(t2) + smoothstep(0.0, 1.0, mod(t2, 1.0)) ;
	float glitch = floor(t/2+0.5);
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	vec3 norm = normal;
	
	mat4 rotationy4 = getyrot(t*0.1 + pos.y*0.02 + index*1.0 + glitch*0.1);
	
	uv = pos.xy + vec2(pos.z, pos.z);
	uv *= 0.1;

	vec3 ppos = pos;
	ppos.y += 0.0 - beat2*0.4;
	ppos.y += index * 20.0;
	//ppos.x += glitch;
	ppos.x += -4.0 + 4.0 * (mod(floor(t/8), 3 + mod(floor(t/9), 2)));
	ppos *= 2.0;
	ppos.x *=  1.0 + sqrt(max(0.0,t-16.0)/30.0) * (0.5+0.5*sin(t + ppos.y))*0.5;
	
	
	ppos = (rotationy4 * vec4(ppos, 1.0)).xyz;
	gl_Position = (projection*cam)*vec4(ppos, 1.0);

	dist=gl_Position.z;
	exNormal = (cam)*vec4(rotationy*normal,.0);
}

#endif

#ifdef fragmentcompile

smooth in vec2 uv;
smooth in vec4 exColor;
smooth in vec4 exNormal;
smooth in float wobble;
smooth in float dist;

layout(location=0) out vec4 outcol;
uniform sampler2D tex;

void main() {
	vec4 tcol = texture(tex, uv);
	tcol.rgb = pow(tcol.rgb, vec3(1.2))*0.9;
	
	//outcol.rgb = tcol.rgb;
	//outcol.a = 1.0;
	outcol.rgba = vec4(1.0, 0.0, 0.0, 1.0);
	//outcol.rgb = exNormal.xyz;
	vec3 light = vec3(-0.8, 0.1, 0.1);
	vec3 normal = -exNormal.xyz;
	vec3 excol = exNormal.xyz;
//	outcol.rgb = max(vec3(0.4), tcol.rgb);
	//outcol.rgb = excol.ggg * vec3(0.8, 0.3, 0.6);
	outcol.rgb = excol.ggg * vec3(0.65, 0.5, 0.5);
	
	//outcol.rgb *= vec3(0.6, 0.5, 0.5)*0.8*excol;
	outcol.rgb *= mix(vec3(0.15,0.2,0.5),vec3(0.7,0.5,0.45), 0.5+.5*max(-1.0, dot(light, normal)) + tcol.r-tcol.g) ;
	
	outcol.rgb = mix(outcol.rgb, excol.rgb, 0.0);
	outcol.rgb = max(outcol.rgb, vec3(0.05));
	outcol.a = min(1.0,40.0/dist);//-gl_FragCoord.z;
	
	}

#endif
