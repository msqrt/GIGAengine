
#pragma once

#include "effect.h"

class Wall : public Effect {
	public:
		Wall();
		~Wall();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
