#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class VuoriEffect: public Effect {
	public:
		VuoriEffect();
		~VuoriEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		mesh quadi;
		//texture mountain;
};