
#pragma once

#include "effect.h"

class Dust : public Effect {
	public:
		Dust();
		~Dust();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
