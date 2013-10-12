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

void NostatusEffect::render(ParameterMap& param)
{
	fill.use();
	simple::tri(-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f + sin(param["t"]));
}
