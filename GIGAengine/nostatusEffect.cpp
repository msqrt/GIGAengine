#include "nostatusEffect.h"
#include "timeline.h"
#include "simpledraw.h"

NostatusEffect::NostatusEffect()
 : Effect(), fill("nostatus.shader"), m(MESH_QUAD)
{

}

NostatusEffect::~NostatusEffect()
{

}

#define M_PI (3.141592653589)

void NostatusEffect::render(ParameterMap& param)
{

	int slices = 8;

	fill.use();

	float add = param["t"] * 0.5;

	for (int i=0;i<slices;i++)  {
		float rad = (2.0*M_PI)/float(slices);

		simple::tri(
			0.0f, 0.0f, 
			0.0f + cos(add + rad*i), 0.0f + sin(add +rad*i), 
			0.0f + cos(add + rad*(i+1)), 0.0f + sin(add + rad*(i+1))
			);
	}
}

#undef M_PI