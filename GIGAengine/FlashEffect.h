
#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class FlashEffect: public Effect {
	public:
		FlashEffect();
		~FlashEffect();
		void render(ParameterMap& param);
	private:
		shader blit;
		mesh quadi;
};