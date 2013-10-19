
#include "main.h"
#include "flashEffect.h"

FlashEffect::FlashEffect() : Effect(), blit("assets/flash.shader"), quadi(MESH_QUAD) {
	
}

FlashEffect::~FlashEffect() { }

void FlashEffect::render(ParameterMap& param) {
	blit.use();

	glUniform1f(blit.getLoc("bright"),param["bright"]);//, param["red"], param["green"], param["blue"]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	quadi.draw(GL_TRIANGLES);
	glDisable(GL_BLEND);
}