
#pragma once

#include "effect.h"

class Sky : public Effect {
	public:
		Sky();
		~Sky();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
