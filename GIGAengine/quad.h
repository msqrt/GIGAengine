
#pragma once

#include "effect.h"

class QuadEffect : public Effect {
	public:
		QuadEffect();
		~QuadEffect();
		void render(SyncMap& sync);
	private:
		shader s;
		mesh m;
};
