
#include "main.h"
#include "blobs.h"

Blobs::Blobs() : Effect(), s("blobs.shader"), m(QUAD){
	
}

Blobs::~Blobs() {}

void Blobs::render(SyncMap& sync) {
	s.use();
	glUniform1f(s.getLoc("t"), sync["globaltime"]);
	m.draw(GL_TRIANGLES);
}
