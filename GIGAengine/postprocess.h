
#pragma once

#define BLOOMSTAGES 5

class PostProcess {
	public:
		PostProcess(int width, int height);
		void bind();
		void render();
	private:
		ParameterMap params;
		mesh quad;
		shader post, gaussian, redux;
		int width, height;
		texture color, additional, depth, bloom, tmpblur;
		target renderTarget[BLOOMSTAGES];
};
