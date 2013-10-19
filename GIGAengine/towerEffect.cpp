#include "main.h"
#include "towerEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"

extern window* win; // defined in main

namespace {
//float screensize[] = {1280.0f, 720.0f};
}

TowerEffect::TowerEffect()
 : Effect(), 
	towershader("assets/tower.shader"), 
	//bg("assets/bgcity.shader"), 
	m(MESH_QUAD),
	cirkula(L"assets/kone.jpg")
	//city(L"assets/walls.png")
{ 
	//fill.addUniform("screenSize", screensize, GVEC2);
	//bg.addUniform("screenSize", screensize, GVEC2);
	tower = util::loadWavefrontObj("assets/meshes/tower.obj", false);
}

TowerEffect::~TowerEffect()
{

}

#define M_PI (3.141592653589)

void TowerEffect::render(ParameterMap& param)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	towershader.use();
	cirkula.bind(0);

	float t = param["t"];
	float mx = win->mousex/1280.0f - 0.5f;
	setCamera(towershader, "camera", -0.5f , 15.0f , 0.5 , 0.1f, .0f, .0f);
	setProjection(towershader, "projection", 6.0f, (float)global_screenh/(float)global_screenw, 0.1, 250.0f);
	towershader.setUniform("t", &t, GFLOAT);
	glEnable(GL_BLEND);
	for (int i=0;i<7;i++) {
		float fi = (float)i;
		towershader.setUniform("index", &fi, GFLOAT);
		tower->draw(GL_TRIANGLES);
	}
	glDisable(GL_BLEND);
	//m.draw(GL_TRIANGLES);
}

#undef M_PI