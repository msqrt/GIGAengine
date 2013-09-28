
#pragma once

#include "effect.h"

class Blobs : public Effect {
	public:
		Blobs();
		~Blobs();
		void render(float t);
	private:
		shader s;
		mesh m;
};
