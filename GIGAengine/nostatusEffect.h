#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class NostatusEffect: public Effect {
	public:
		NostatusEffect();
		~NostatusEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		shader bg;
		mesh m;
		mesh* suippo;
		texture cirkula;
		texture city;
		int sliceindex;
};
