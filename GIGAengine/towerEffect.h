#pragma once

#include "effect.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "timeline.h"

class TowerEffect: public Effect {
	public:
		TowerEffect();
		~TowerEffect();
		void render(ParameterMap& param);
	private:
		shader towershader;
		mesh quadi;
		//texture tex;
		texture cirkula;
		mesh* tower;
		mesh m;
		//mesh* pyrpyla;
};