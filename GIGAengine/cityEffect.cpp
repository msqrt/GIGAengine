
#include "main.h"
#include "cityeffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"
#include "camera.h"

extern texture* cityTexture;

namespace {
float screensize[] = {1280.0f, 720.0f};
}

CityEffect::CityEffect()
 : Effect(), 
	fill("assets/city.shader"), 
	quadi(MESH_QUAD),
	mountain(L"assets/walls.png")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	//bgball = util::loadWavefrontObj("assets/meshes/boltsi.obj", false);
	//suippo = util::loadWavefrontObj("assets/meshes/suippo.obj", true);
}

CityEffect::~CityEffect()
{
	//delete bgball;
}

#define M_PI (3.141592653589)

void CityEffect::render(ParameterMap& param)
{
	fill.use();	
	fill.setUniform("screenSize", screensize, GVEC2);
	fill.setUniform("t", &param["t"], GFLOAT);
	fill.setUniform("lamp", &param["lamp"], GFLOAT);
	//mountain.bind(0);	
	cityTexture->bind(0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);
	quadi.draw(GL_TRIANGLES);
	glDisable(GL_BLEND);

}