
#include "main.h"
#include "quad.h"

QuadEffect::QuadEffect() : Effect(), s("quad.shader"), m(QUAD) {
}

QuadEffect::~QuadEffect() {}

void QuadEffect::render(ParameterMap& param) {
	s.use();
	m.draw(GL_TRIANGLES);
}