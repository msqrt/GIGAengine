
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
	setCamera(s, "camera", param["camx"]*.01f, param["camy"]*.01f, param["camz"]*.001f, param["targetx"]*.01f, param["targety"]*.01f, param["targetz"]*.01f);
	setProjection(s, "projection", param["fov"], param["aspect"]);
	glUniform1f(s.getLoc("t"), param["t"]);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	m.draw(GL_POINTS);
	//glClear(GL_DEPTH_BUFFER_BIT);
}
