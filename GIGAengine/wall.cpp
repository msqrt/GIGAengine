
#include "main.h"
#include "wall.h"
#include "camera.h"

#define RES 512

Wall::Wall() : Effect(), s("assets/wall.shader") {
	float * geom = new float[RES*RES*7];
	for(int x = 0; x<RES; x++)
	for(int y = 0; y<RES; y++) {
		float dx = int(x-float(RES-1)/2.0f)/float(RES-2);
		float dy = int(y-float(RES-1)/2.0f)/float(RES-2);
		float l = 1.0f/sqrt(dx*dx+dy*dy+1.0f);
		geom[(x*RES+y)*7+0] = dx*l;
		geom[(x*RES+y)*7+1] = dy*l;
		geom[(x*RES+y)*7+2] = 1.0f-l;
		geom[(x*RES+y)*7+3] = rand()/float(RAND_MAX);
		geom[(x*RES+y)*7+4] = rand()/float(RAND_MAX);
		geom[(x*RES+y)*7+5] = rand()/float(RAND_MAX);
		geom[(x*RES+y)*7+6] = rand()/float(RAND_MAX);
	}

	m.data(RES*RES, 7*sizeof(float), geom);
	delete [] geom;

	unsigned * inds = new unsigned[(RES-1)*(RES-1)*6];
	for(int x = 0; x<RES-1; x++)
	for(int y = 0; y<RES-1; y++) {
		inds[(x*(RES-1)+y)*6+0] = (x  )*RES+y  ;
		inds[(x*(RES-1)+y)*6+1] = (x+1)*RES+y+1;
		inds[(x*(RES-1)+y)*6+2] = (x+1)*RES+y  ;
		inds[(x*(RES-1)+y)*6+3] = (x  )*RES+y  ;
		inds[(x*(RES-1)+y)*6+4] = (x  )*RES+y+1;
		inds[(x*(RES-1)+y)*6+5] = (x+1)*RES+y+1;
	}
	
	m.indices((RES-1)*(RES-1)*6, inds);
	delete [] inds;

	m.pointer(0, 0, 3);
	m.pointer(1, 3, 4);

}

Wall::~Wall() {}

void Wall::render(ParameterMap& param) {
	s.use();
	setCamera(s, "camera", param["camx"], param["camy"], param["camz"], param["targetx"], param["targety"], param["targetz"]);
	setProjection(s, "projection", param["fov"], param["aspect"]);
	glUniform3f(s.getLoc("primaryDirection"), param["primx"], param["primy"], param["primz"]);
	glUniform3f(s.getLoc("primaryColour"), param["primh"], param["prims"], param["primv"]);
	glUniform3f(s.getLoc("secondaryDirection"), param["secx"], param["secy"], param["secz"]);
	glUniform3f(s.getLoc("secondaryColour"), param["sech"], param["secs"], param["secv"]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m.draw(GL_TRIANGLES);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_DEPTH_BUFFER_BIT);
}
