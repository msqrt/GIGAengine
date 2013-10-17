#include "main.h"
#include "blurbEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"
#include "shaderstorage.h"

namespace {
float screensize[] = {1280.0f, 720.0f};
}

BlurbEffect::BlurbEffect()
 : Effect(), 
	fill("assets/mollykka.shader"), 
	quadi(MESH_QUAD),
	tex(L"assets/kone.jpg")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	shape = util::loadWavefrontObj("assets/meshes/mollykka.obj", false);
	//pyrpyla = util::loadWavefrontObj("assets/meshes/yrpyla.obj", false);
}

BlurbEffect::~BlurbEffect()
{
	delete shape;
}

#define M_PI (3.141592653589)

void BlurbEffect::render(ParameterMap& param)
{
	glClear(GL_DEPTH_BUFFER_BIT);	// be nasty and clear the depth buffer
	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);
	fill.setUniform("speed", &param["speed"], GFLOAT);

	setCamera(fill, "camera", 3.0f, -4.0f, -0.0f, 0.0f, 25.0f, .0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

	tex.bind(0);	
	shape->draw(GL_TRIANGLES);

	setCamera(fill, "camera", 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

}