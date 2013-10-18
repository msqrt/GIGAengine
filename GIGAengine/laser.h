
#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class LaserEffect: public Effect {
	public:
		LaserEffect(std::wstring filename);
		~LaserEffect();
		void render(ParameterMap& param);
	private:
		shader blit;
		mesh quadi;
		texture tex;
};