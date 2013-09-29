
#pragma once

#include "effect.h"

class Blobs : public Effect {
	public:
		Blobs();
		~Blobs();
		void render(ParameterMap& param);
	private:
		shader s;
		mesh m;
};
