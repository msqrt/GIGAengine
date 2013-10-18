
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
	float beat = floor(t) + smoothstep(0.0, 1.0, mod(t, 1.0)) ;
	
	mat3 rotationy = mat3(
		cos(alpha), 0.0, sin(alpha),
		0.0, 1.0, 0.0,
		-sin(alpha), 0.0, cos(alpha)
	);
	vec3 norm = normal;
	
	mat4 rotationy4 = getyrot(t*0.1 + extrarotation * 1.0);

	vec3 ppos = pos;
	ppos.y += 0.0 - t*0.2;
	ppos.y += index * 10.0;
	ppos *= 2.0;
	ppos = (rotationy4 * vec4(ppos, 1.0)).xyz;
	gl_Position = (projection*cam)*vec4(ppos, 1.0);
	exNormal = (cam)*vec4(rotationy*normal,.0);
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
	vec4 tcol = texture(tex, uv);

	//outcol.rgb = tcol.rgb;
	//outcol.a = 1.0;
	outcol.rgba = vec4(1.0, 0.0, 0.0, 1.0);
	outcol.rgb = exNormal.xyz;
	}

#endif
