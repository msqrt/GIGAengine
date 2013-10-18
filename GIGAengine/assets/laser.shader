
#ifdef vertexcompile

layout (location=0) in vec2 pos;

uniform vec2 upos;
uniform float aspect;
uniform float uang;
uniform float uscale;

smooth out vec2 uv;

void main() {
	uv = vec2(.5)+.5*pos;
	mat2 rot = mat2(cos(uang), sin(uang), -sin(uang), cos(uang));
	gl_Position = vec4(upos+rot*pos*vec2(aspect,1.0)*uscale, .6,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 uv;
uniform float bright, bright2;
uniform sampler2D tex;

void main() {
	vec4 col =bright*texture(tex, uv);
	float col2 = max(1,col.x)+max(1,col.z)+max(1,col.z)-3;
	vec4 col3 = col+vec4(col2);
	outcol = clamp(vec4(vec3(bright2),length(col.xyz)),vec4(.0),vec4(1.0));
}

#endif
