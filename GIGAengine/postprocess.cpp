
#include "main.h"
#include "effect.h"
#include "postprocess.h"

PostProcess::PostProcess(int width, int height) :
	width(width),
	height(height),
	quad(QUAD),
	post("postprocess.shader"),
	gaussian("gaussian.shader"),
	redux("redux.shader"),
	color(width, height, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
	additional(width, height, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
	depth(width, height, true, GL_NEAREST, GL_CLAMP_TO_BORDER),
	bloom(width, height, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
	tmpblur(width, height, false, GL_NEAREST, GL_CLAMP_TO_BORDER) {
	
	glUniform1i(gaussian.getLoc("source"), 0);

	bloom.bind(0);
	glGenerateMipmap(GL_TEXTURE_2D);
	tmpblur.bind(0);
	glGenerateMipmap(GL_TEXTURE_2D);

	for(int i = 0; i<BLOOMSTAGES; i++) {
		renderTarget[i].attach(width, height);
		renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i);
		if(i==0) {
			renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &color, false, i);
			renderTarget[i].setTexture(GL_COLOR_ATTACHMENT1, &additional, false, i);
			renderTarget[i].setTexture(GL_DEPTH_ATTACHMENT, &depth, false, i);
		}
	}
}

void PostProcess::bind() {
	renderTarget[0].attach(width, height);
}

void PostProcess::render() {
	
	renderTarget[0].attach(width, height);
	
	int curwidth = width, curheight = height;

	glDisable(GL_DEPTH_TEST);

	color.bind(0);

	for(int i = 0; i<BLOOMSTAGES; i++) {
		renderTarget[i].attach(curwidth, curheight);
		renderTarget[0].setTexture(GL_COLOR_ATTACHMENT0, &tmpblur, false, i);

		gaussian.use();
		glUniform1f(gaussian.getLoc("dir"), .0f);
		quad.draw(GL_TRIANGLES);
		
		renderTarget[0].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i);
		tmpblur.bind(0);

		glUniform1f(gaussian.getLoc("dir"), 1.0f);
		quad.draw(GL_TRIANGLES);
		redux.use();
		
		if(i+1<BLOOMSTAGES) {
			curwidth  /= 2;
			curheight /= 2;

			renderTarget[0].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i);

		}
		quad.draw(GL_TRIANGLES);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	post.use();
	quad.draw(GL_TRIANGLES);
	renderTarget[0].attach(width, height);
	renderTarget[0].setTexture(GL_COLOR_ATTACHMENT0, &color, false, 0);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
}