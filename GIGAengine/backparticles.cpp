
#include "main.h"
#include "backparticles.h"
#include "camera.h"

#define COUNT 60000

Dust::Dust() : Effect(), s("assets/dust.shader") {
	float * geom = new float[COUNT*10];
	float r1 = rand()/float(RAND_MAX), r2 = rand()/float(RAND_MAX), r3 = rand()/float(RAND_MAX);
	for(int i = 0; i<COUNT; i++) {
		geom[i*10+0] = rand()/float(RAND_MAX);
		geom[i*10+1] = rand()/float(RAND_MAX);
		geom[i*10+2] = rand()/float(RAND_MAX);
		geom[i*10+3] = rand()/float(RAND_MAX);
		geom[i*10+4] = r1;
		geom[i*10+5] = r2;
		geom[i*10+6] = r3;
		geom[i*10+7] = rand()/float(RAND_MAX);
		geom[i*10+8] = rand()/float(RAND_MAX);
		geom[i*10+9] = rand()/float(RAND_MAX);
	}

	m.data(COUNT, 10*sizeof(float), geom);
	delete [] geom;

	m.pointer(0, 0, 4);
	m.pointer(1, 4, 3);
	m.pointer(2, 7, 3);
}

Dust::~Dust() {}


void Dust::render(ParameterMap& param) {
	s.use();
	setCamera(s, "camera", -0.5f , 15.0f , 0.5 , 0.1f, .0f, .0f);
	setProjection(s, "projection", 6.0f, (float)global_screenh/(float)global_screenw, 0.1, 250.0f);
	glUniform1f(s.getLoc("t"), param["t"]);
	glUniform1f(s.getLoc("size"), param["size"]);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	m.draw(GL_POINTS);
}
