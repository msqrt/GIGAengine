
#pragma once

#include "effect.h"

class QuadEffect : public Effect {
	public:
		QuadEffect();
		~QuadEffect();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
