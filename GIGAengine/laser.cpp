
#include "main.h"
#include "laser.h"

LaserEffect::LaserEffect(std::wstring filename) : Effect(), tex(filename, GL_LINEAR), blit("assets/laser.shader"), quadi(MESH_QUAD) {
	
}

LaserEffect::~LaserEffect() { }

void LaserEffect::render(ParameterMap& param) {
	blit.use();
	glClear(GL_DEPTH_BUFFER_BIT);
	glUniform2f(blit.getLoc("upos"), param["x"], param["y"]);
	glUniform1f(blit.getLoc("aspect"), param["aspect"]);
	glUniform1f(blit.getLoc("uang"), param["ang"]);
	glUniform1f(blit.getLoc("uscale"), param["scale"]);
	glUniform1f(blit.getLoc("bright"), param["bright"]);
	glUniform1f(blit.getLoc("bright2"),param["bright2"]);
	glUniform1i(blit.getLoc("tex"), 0);
	tex.bind(0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	quadi.draw(GL_TRIANGLES);
	glDisable(GL_BLEND);
}