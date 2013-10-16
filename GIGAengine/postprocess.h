
#pragma once

#define BLOOMSTAGES 8

class PostProcess {
	public:
		PostProcess(int width, int height);
		void bind();
		void render(float t);
	private:
		ParameterMap params;
		mesh quad;
		shader post, gaussian, redux;
		int width, height;
		texture color, additional, depth, bloom, tmpblur;
		target renderTarget[BLOOMSTAGES];
};
