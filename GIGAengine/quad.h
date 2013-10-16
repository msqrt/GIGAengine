
#pragma once

#include "effect.h"

class Quad : public Effect {
	public:
		Quad();
		~Quad();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
