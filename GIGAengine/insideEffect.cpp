#include "main.h"
#include "insideEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"
#include "shaderstorage.h"

extern texture* koneTexture;

namespace {
float screensize[] = {1280.0f, 720.0f};
}

InsideEffect::InsideEffect()
 : Effect(), 
	fill("assets/bgball.shader"), 
	quadi(MESH_QUAD)
	//tex(L"assets/kone.jpg")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	ball = util::loadWavefrontObj("assets/meshes/boltsi.obj", false);
	//pyrpyla = util::loadWavefrontObj("assets/meshes/yrpyla.obj", false);
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
	fill.setUniform("speed", &param["speed"], GFLOAT);

	setCamera(fill, "camera", 3.0f, -4.0f, -0.0f, 0.0f, 25.0f, .0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

	koneTexture->bind(0);	
	ball->draw(GL_TRIANGLES);

	setCamera(fill, "camera", 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

}