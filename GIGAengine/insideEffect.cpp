#include "main.h"
#include "insideEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"

namespace {
float screensize[] = {1280.0f, 720.0f};
}

InsideEffect::InsideEffect()
 : Effect(), 
	fill("assets/bgball.shader"), 
	quadi(MESH_QUAD),
	tex(L"assets/vuori.jpg")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	ball = util::loadWavefrontObj("assets/meshes/boltsi.obj", false);
}

InsideEffect::~InsideEffect()
{
	delete ball;
}

#define M_PI (3.141592653589)

void InsideEffect::render(ParameterMap& param)
{
	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);

	setCamera(fill, "camera", 3.0f, -2.0f, -2.0f, 0.0f, .0f, .0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

	tex.bind(0);	
	ball->draw(GL_TRIANGLES);
}