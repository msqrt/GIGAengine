
#include "main.h"
#include "vuoriEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"

extern texture* mountainTexture;

namespace {
float screensize[] = {1280.0f, 720.0f};
}

VuoriEffect::VuoriEffect()
 : Effect(), 
	fill("assets/bgvuori.shader"), 
	quadi(MESH_QUAD)
	//mountain(L"assets/vuori.jpg")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	//suippo = util::loadWavefrontObj("assets/meshes/suippo.obj", true);
}

VuoriEffect::~VuoriEffect()
{

}

#define M_PI (3.141592653589)

void VuoriEffect::render(ParameterMap& param)
{
	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);
	fill.setUniform("lamp", &param["lamp"], GFLOAT);
	mountainTexture->bind(0);	
	quadi.draw(GL_TRIANGLES);
}