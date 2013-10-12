#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "timeline.h"

class NostatusEffect: public Effect {
	public:
		NostatusEffect();
		~NostatusEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		mesh m;
		int sliceindex;
};
