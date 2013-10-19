#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class CreditsEffect: public Effect {
	public:
		CreditsEffect(std::wstring texture_path);
		~CreditsEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		mesh quadi;
		texture tex;
		// mesh* ball;
		// mesh* pyrpyla;
};