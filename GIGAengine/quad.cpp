
#include "main.h"
#include "quad.h"

QuadEffect::QuadEffect() : Effect(), s("quad.shader"), m(QUAD) {
}

QuadEffect::~QuadEffect() {}

void QuadEffect::render(SyncMap& sync) {
	s.use();
	glUniform1f(s.getLoc("t"), sync["time"]);
	m.draw(GL_TRIANGLES);
}