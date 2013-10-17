#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class BlurbEffect: public Effect {
	public:
		BlurbEffect();
		~BlurbEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		shader billboard;
		mesh quadi;
		//texture tex;
		mesh* shape;
};