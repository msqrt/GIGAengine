
#include "main.h"
#include "blobs.h"

Blobs::Blobs() : Effect(), s("blobs.shader"), m(QUAD){
	
}

Blobs::~Blobs() {}

void Blobs::render(float t) {
	s.use();
	m.draw(GL_TRIANGLES);
}
