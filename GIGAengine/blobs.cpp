
#include "main.h"
#include "blobs.h"

Blobs::Blobs() : Effect(), s("blobs.shader"), m(QUAD){
	
}

Blobs::~Blobs() {}

void Blobs::render(ParameterMap& param) {
	s.use();
	glUniform1f(s.getLoc("t"), param["t"]);
	glUniform3f(s.getLoc("col"), param["r"], param["g"], param["b"]);
	m.draw(GL_TRIANGLES);
}
