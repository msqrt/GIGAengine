#include "nostatusEffect.h"
#include "timeline.h"
#include "simpledraw.h"
#include "objloader.h"

namespace {
float screensize[] = {1280.0f, 720.0f};
}

NostatusEffect::NostatusEffect()
 : Effect(), 
	fill("assets/nostatus.shader"), 
	m(MESH_QUAD),
	cirkula(L"assets/cirkula2.jpg")
{ 
	fill.addUniform("screenSize", screensize, GVEC2);
	suippo = util::loadWavefrontObj("assets/meshes/suippo.obj", true);
}

NostatusEffect::~NostatusEffect()
{

}

#define M_PI (3.141592653589)

void NostatusEffect::render(ParameterMap& param)
{

	float ratio = 16.0f/9.0f;
	int slices = 8;

	fill.use();
	cirkula.bind(0);
	glUniform1i(fill.getLoc("tex"), 0);
	glUniform1f(fill.getLoc("t"), param["t"]);

	float add = 0.0f;

	for (int i=0;i<slices;i++)  {
		float rad = (2.0*M_PI)/float(slices);

		glUniform1i(fill.getLoc("index"), i);

		/*
		simple::tri(
			0.0f, 0.0f, 
			0.0f + cos(add + rad*i), 0.0f + sin(add +rad*i), 
			0.0f + cos(add + rad*(i+1)), 0.0f + sin(add + rad*(i+1))
			);
		*/
	}

	suippo->draw(GL_TRIANGLES);
}

#undef M_PI