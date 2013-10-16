
#include "main.h"
#include "quad.h"

Quad::Quad() : Effect(), s("assets/quad.shader"), m(QUAD) {
	
}

Quad::~Quad() {}

void Quad::render(ParameterMap& param) {
	s.use();
	glUniform1f(s.getLoc("t"), param["t"]);
	m.draw(GL_TRIANGLES);
}
