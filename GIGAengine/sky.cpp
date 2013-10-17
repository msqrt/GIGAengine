
#include "main.h"
#include "sky.h"
#include "camera.h"

#define OBJS 1
#define RES 4

Sky::Sky() : Effect(), s("assets/sky.shader") {
	float * geom = new float[OBJS*6*RES*RES*3];
	for(int i = 0; i<OBJS; i++) {
		for(int j = 0; j<6; j++) {
			float nx = 1.0f*(j==0)-1.0f*(j==1);
			float ny = 1.0f*(j==2)-1.0f*(j==3);
			float nz = 1.0f*(j==4)-1.0f*(j==5);
			float ox = nx*.5f;
			float oy = ny*.5f;
			float oz = nz*.5f;
			float xdx = .0f, ydx = .0f, zdx = .0f, xdy = .0f, ydy = .0f, zdy = .0f;
			switch(j) {
				case 0: zdx = 1.0f; ydy = -1.0f; break;
				case 1: zdx = 1.0f; ydy = 1.0f; break;
				case 2: zdx = 1.0f; xdy = 1.0f; break;
				case 3: zdx = 1.0f; xdy = -1.0f; break;
				case 4: xdx = 1.0f; ydy = 1.0f; break;
				case 5: xdx = 1.0f; ydy = -1.0f; break;
			}
			for(int x = 0; x<RES; x++)
			for(int y = 0; y<RES; y++) {
				float dx = int(x-float(RES-1)/2.0f)/float(RES-2);
				float dy = int(y-float(RES-1)/2.0f)/float(RES-2);
				geom[(((i*6+j)*RES+x)*RES+y)*3+ 0] = ox+dx*xdx+dy*xdy;
				geom[(((i*6+j)*RES+x)*RES+y)*3+ 1] = oy+dx*ydx+dy*ydy;
				geom[(((i*6+j)*RES+x)*RES+y)*3+ 2] = oz+dx*zdx+dy*zdy;
			}
		}
	}

	m.data(OBJS*6*RES*RES, 3*sizeof(float), geom);
	delete [] geom;

	unsigned * inds = new unsigned[OBJS*6*(RES-1)*(RES-1)*6];
	for(int i = 0; i<OBJS; i++)
	for(int j = 0; j<6; j++)
	for(int x = 0; x<RES-1; x++)
	for(int y = 0; y<RES-1; y++) {
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+0] = ((i*6+j)*RES+x  )*RES+y  ;
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+1] = ((i*6+j)*RES+x+1)*RES+y  ;
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+2] = ((i*6+j)*RES+x+1)*RES+y+1;
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+3] = ((i*6+j)*RES+x  )*RES+y  ;
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+4] = ((i*6+j)*RES+x+1)*RES+y+1;
		inds[(((i*6+j)*(RES-1)+x)*(RES-1)+y)*6+5] = ((i*6+j)*RES+x  )*RES+y+1;
	}
	
	m.indices(OBJS*6*(RES-1)*(RES-1)*6, inds);
	delete [] inds;

	m.pointer(0, 0, 3);
	m.pointer(1, 3, 3);
	m.pointer(2, 6, 4);
	m.pointer(3,10, 4);

}

Sky::~Sky() {}


void Sky::render(ParameterMap& param) {
	s.use();
	setCamera(s, "camera", .0f, .0f, .0f, param["targetx"]-param["camx"], param["targety"]-param["camy"], param["targetz"]-param["camz"]);
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
