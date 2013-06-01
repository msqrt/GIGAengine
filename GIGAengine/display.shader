
#ifdef vertexcompile

layout (location=0) in vec2 ipos;
smooth out vec2 npos;
void main()
{
	npos = ipos*.5+vec2(.5);
	gl_Position = vec4(ipos,.0,1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
smooth in vec2 npos;

void main()
{
	outcol = vec4(npos.xy, .0, .0);//vec4(.5+.25*(sin(npos.x*60.0)+sin(npos.y*60.0)));
}

#endif
