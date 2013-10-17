#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class InsideEffect: public Effect {
	public:
		InsideEffect();
		~InsideEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		mesh quadi;
		texture tex;
		mesh* ball;
};