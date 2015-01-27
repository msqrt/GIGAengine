
#ifdef vertexcompile

layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

uniform mat4 model_to_world;
uniform mat4 world_to_cam;
uniform mat4 cam_to_clip;

flat out float shade;
smooth out vec3 pos;
void main() {
	shade = .5*(1.0+dot(normalize(a_normal), normalize(vec3(.1, .2, .8))));
	//shade = shade*shade;
	pos = vec4(transpose(world_to_cam) * transpose(model_to_world) * vec4(a_position,1.0)).xyz;
	gl_Position = transpose(cam_to_clip) *vec4(pos, 1.0);
}

#endif

#ifdef fragmentcompile

layout (location=0) out vec4 outcol;
flat in float shade;
smooth in vec3 pos;

void main() {
	outcol = vec4(shade)*max(.0,exp(-.02*pos.z));
}

#endif
