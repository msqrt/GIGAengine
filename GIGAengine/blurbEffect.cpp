#include "main.h"
#include "blurbEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"
#include "shaderstorage.h"

extern texture* koneTexture;
extern texture* mountainTexture;

namespace {
float screensize[] = {1280.0f, 720.0f};
}

BlurbEffect::BlurbEffect()
 : Effect(), 
	fill("assets/mollykka.shader"), 
	billboard("assets/altvuori.shader"), 
	quadi(MESH_QUAD)
	//tex(L"assets/kone.jpg")
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
	billboard.use();
	billboard.setUniform("screenSize", screensize, GVEC2);
	billboard.setUniform("t", &param["t"], GFLOAT);
	billboard.setUniform("speed", &param["speed"], GFLOAT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mountainTexture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	quadi.draw(GL_TRIANGLES);
	glDisable(GL_BLEND);

	glClear(GL_DEPTH_BUFFER_BIT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);
	fill.setUniform("speed", &param["speed"], GFLOAT);

	setCamera(fill, "camera", 3.0f, -4.0f, -0.0f, 0.0f, 25.0f, .0f);
	setProjection(fill, "projection", 6.0f, 9.0/16.0);

	koneTexture->bind(0);	
	shape->draw(GL_TRIANGLES);
}