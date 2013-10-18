
#include "main.h"
#include "laser.h"

LaserEffect::LaserEffect(std::wstring filename) : Effect(), tex(filename, GL_LINEAR), blit("laser.shader"), quadi(MESH_QUAD) {
	
}

LaserEffect::~LaserEffect() { }

void LaserEffect::render(ParameterMap& param) {
	blit.use();

	glUniform2f(blit.getLoc("upos"), param["x"], param["y"]);
	glUniform1f(blit.getLoc("uang"), param["ang"]);
	glUniform1f(blit.getLoc("uscale"), param["scale"]);
	glUniform1f(blit.getLoc("bright"), param["bright"]);
	glUniform1f(blit.getLoc("t"), param["t"]);
	glUniform1i(blit.getLoc("tex"), 0);
	tex.bind(0);

	quadi.draw(GL_TRIANGLES);
}