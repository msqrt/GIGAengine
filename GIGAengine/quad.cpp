
#include "main.h"
#include "quad.h"

QuadEffect::QuadEffect() : Effect(), s("assets/quad.shader"), m(QUAD) {
}

QuadEffect::~QuadEffect() {}

void QuadEffect::render(ParameterMap& param) {
	s.use();
	glUniform1f(s.getLoc("t"), param["t"]);
	m.draw(GL_TRIANGLES);
}
