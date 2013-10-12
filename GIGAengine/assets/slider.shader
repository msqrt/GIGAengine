
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 npos;
void main() {
	npos = ipos*.5+vec2(.5);
	gl_Position = vec4(ipos.x*.9, ipos.y*.05-.85,.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 npos;
uniform float hover, t;

void main() {
	float col = min(1.0,max(.0,1.9-800.0*abs(npos.x-t)));
	if(abs(npos.y-.5)<.05)
		col = 1.0;
	outcol = col*vec4(.1+.3*hover);
}

#endif
