#include "main.h"
#include "creditsEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"
#include "shaderstorage.h"

extern texture* koneTexture;

namespace {
float screensize[] = {1280.0f, 720.0f};
}

CreditsEffect::CreditsEffect()
 : Effect(), 
	fill("assets/texture.shader"), 
	quadi(MESH_QUAD),
	tex(L"assets/credits.png")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);

	//pyrpyla = util::loadWavefrontObj("assets/meshes/yrpyla.obj", false);
}

CreditsEffect::~CreditsEffect()
{
}

#define M_PI (3.141592653589)

void CreditsEffect::render(ParameterMap& param)
{
	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);
	fill.setUniform("speed", &param["speed"], GFLOAT);

	tex.bind(0);

	quadi.draw(GL_TRIANGLES);
}