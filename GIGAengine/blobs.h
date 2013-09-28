
#pragma once

#include "effect.h"

class Blobs : public Effect {
	public:
		Blobs();
		~Blobs();
		void render(SyncMap& l);
	private:
		shader s;
		mesh m;
};
