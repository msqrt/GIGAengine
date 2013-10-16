
#pragma once

#include "effect.h"

class BackGround : public Effect {
	public:
		BackGround();
		~BackGround();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
