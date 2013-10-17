#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class CityEffect: public Effect {
	public:
		CityEffect();
		~CityEffect();
		void render(ParameterMap& param);
	private:
		shader fill;
		mesh quadi;
		mesh* bgball;
		texture mountain;
};